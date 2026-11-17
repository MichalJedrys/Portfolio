#include "playruntime/runtime.hpp"

#include <gtest/gtest.h>

#include <array>
#include <limits>
#include <stdexcept>

namespace {
using namespace playruntime;

constexpr std::array rules{LightRule{EventKind::button, LightColour::cyan},
                           LightRule{EventKind::colour, LightColour::blue},
                           LightRule{EventKind::motion, LightColour::green}};

TEST(Runtime, ButtonProducesStructuredLightCommandAtItsEventTime) {
    Runtime runtime{rules};
    const std::array events{Event{25, 0, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_EQ(runtime.pending_events(), 1U);
    EXPECT_FALSE(runtime.closed_horizon());
    const auto result = runtime.advance_to(100);
    EXPECT_FALSE(result.error);
    EXPECT_EQ(result.commands, (std::vector<CommandRecord>{{25, 0, SetLight{LightColour::cyan}}}));
    EXPECT_EQ(runtime.pending_events(), 0U);
    EXPECT_EQ(runtime.closed_horizon(), 100);
}

TEST(Runtime, OrdersDifferentEventTypesByTimeThenSequenceAcrossBatches) {
    Runtime runtime{rules};
    const std::array first{Event{30, 10, MotionDetected{}}, Event{10, 11, ButtonPressed{}}};
    const std::array second{Event{10, 12, ColourDetected{SensorColour::blue}},
                            Event{5, 13, MotionDetected{}}};
    ASSERT_FALSE(runtime.submit_batch(first));
    ASSERT_FALSE(runtime.submit_batch(second));
    const auto result = runtime.advance_to(30);
    ASSERT_FALSE(result.error);
    EXPECT_EQ(result.commands,
              (std::vector<CommandRecord>{{5, 13, SetLight{LightColour::green}},
                                          {10, 11, SetLight{LightColour::cyan}},
                                          {10, 12, SetLight{LightColour::blue}},
                                          {30, 10, SetLight{LightColour::green}}}));
}

TEST(Runtime, HorizonIsInclusiveAndFutureEventsRemainQueued) {
    Runtime runtime{rules};
    const std::array events{Event{0, 0, ButtonPressed{}}, Event{10, 1, MotionDetected{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    const auto first = runtime.advance_to(0);
    ASSERT_EQ(first.commands.size(), 1U);
    EXPECT_EQ(first.commands.front().timestamp, 0);
    EXPECT_EQ(runtime.pending_events(), 1U);
    const auto repeated = runtime.advance_to(0);
    EXPECT_FALSE(repeated.error);
    EXPECT_TRUE(repeated.commands.empty());
    EXPECT_TRUE(runtime.advance_to(9).commands.empty());
    EXPECT_EQ(runtime.advance_to(10).commands.size(), 1U);
}

TEST(Runtime, EmptyAdvanceClosesHorizonAndLateInputIsRejected) {
    Runtime runtime{rules};
    ASSERT_FALSE(runtime.submit_batch({}));
    ASSERT_FALSE(runtime.advance_to(5).error);
    for (Timestamp timestamp : {0, 5}) {
        const std::array late{Event{timestamp, 0, ButtonPressed{}}};
        EXPECT_EQ(runtime.submit_batch(late), (InputError{ErrorCode::closed_horizon, 0}));
    }
    const std::array valid{Event{6, 0, ButtonPressed{}}};
    EXPECT_FALSE(runtime.submit_batch(valid));
}

TEST(Runtime, InvalidAdvancePreservesQueueAndClock) {
    Runtime runtime{rules};
    const std::array events{Event{10, 1, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_EQ(runtime.advance_to(-1).error, (InputError{ErrorCode::negative_time, std::nullopt}));
    EXPECT_FALSE(runtime.closed_horizon());
    ASSERT_FALSE(runtime.advance_to(5).error);
    const auto result = runtime.advance_to(4);
    EXPECT_EQ(result.error, (InputError{ErrorCode::closed_horizon, std::nullopt}));
    EXPECT_TRUE(result.commands.empty());
    EXPECT_EQ(runtime.closed_horizon(), 5);
    EXPECT_EQ(runtime.pending_events(), 1U);
    EXPECT_EQ(runtime.advance_to(10).commands.size(), 1U);
}

TEST(Runtime, NegativeTimestampRejectsWholeBatchWithoutConsumingSequenceIds) {
    Runtime runtime{rules};
    const std::array invalid{Event{1, 0, ButtonPressed{}}, Event{-1, 1, MotionDetected{}}};
    EXPECT_EQ(runtime.submit_batch(invalid), (InputError{ErrorCode::negative_time, 1}));
    EXPECT_EQ(runtime.pending_events(), 0U);
    const std::array valid{Event{1, 0, ButtonPressed{}}};
    EXPECT_FALSE(runtime.submit_batch(valid));
}

TEST(Runtime, InvalidColourRejectsWholeBatchAndPreservesExistingEvents) {
    Runtime runtime{rules};
    const std::array existing{Event{20, 3, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(existing));
    const std::array invalid{Event{5, 4, MotionDetected{}},
                             Event{6, 5, ColourDetected{static_cast<SensorColour>(99)}}};
    EXPECT_EQ(runtime.submit_batch(invalid), (InputError{ErrorCode::invalid_payload, 1}));
    EXPECT_EQ(runtime.pending_events(), 1U);
    const std::array valid{Event{5, 4, MotionDetected{}}};
    ASSERT_FALSE(runtime.submit_batch(valid));
    EXPECT_EQ(runtime.advance_to(20).commands,
              (std::vector<CommandRecord>{{5, 4, SetLight{LightColour::green}},
                                          {20, 3, SetLight{LightColour::cyan}}}));
}

TEST(Runtime, DuplicateAndDecreasingSequenceIdsAreRejectedAtomically) {
    for (SequenceId second : {0U, 1U}) {
        Runtime runtime{rules};
        const std::array invalid{Event{1, 1, ButtonPressed{}}, Event{2, second, MotionDetected{}}};
        EXPECT_EQ(runtime.submit_batch(invalid), (InputError{ErrorCode::invalid_sequence, 1}));
        EXPECT_EQ(runtime.pending_events(), 0U);
    }
}

TEST(Runtime, SequenceWatermarkSurvivesProcessingAndEmptyBatches) {
    Runtime runtime{rules};
    const std::array initial{Event{0, 7, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(initial));
    ASSERT_FALSE(runtime.advance_to(0).error);
    ASSERT_FALSE(runtime.submit_batch({}));
    const std::array repeated{Event{1, 7, MotionDetected{}}};
    EXPECT_EQ(runtime.submit_batch(repeated), (InputError{ErrorCode::invalid_sequence, 0}));
}

TEST(Runtime, UnmatchedEventsAreConsumedAndFirstMatchingRuleWins) {
    const std::array button_rules{LightRule{EventKind::button, LightColour::cyan},
                                  LightRule{EventKind::button, LightColour::red}};
    Runtime runtime{button_rules};
    const std::array events{Event{0, 0, MotionDetected{}}, Event{0, 1, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_EQ(runtime.advance_to(0).commands,
              (std::vector<CommandRecord>{{0, 1, SetLight{LightColour::cyan}}}));
    EXPECT_EQ(runtime.pending_events(), 0U);
}

TEST(Runtime, EmptyProgramConsumesEventsWithoutCommands) {
    Runtime runtime{{}};
    const std::array events{Event{0, 0, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_TRUE(runtime.advance_to(0).commands.empty());
    EXPECT_EQ(runtime.pending_events(), 0U);
}

TEST(Runtime, ConstructorRejectsInvalidRulesAndOwnsItsConfiguration) {
    const std::array invalid_kind{LightRule{static_cast<EventKind>(99), LightColour::cyan}};
    const std::array invalid_colour{LightRule{EventKind::button, static_cast<LightColour>(99)}};
    EXPECT_THROW((Runtime{invalid_kind}), std::invalid_argument);
    EXPECT_THROW((Runtime{invalid_colour}), std::invalid_argument);
    std::array configuration{LightRule{EventKind::button, LightColour::cyan}};
    Runtime runtime{configuration};
    configuration[0].colour = LightColour::red;
    const std::array events{Event{0, 0, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_EQ(runtime.advance_to(0).commands.front().command.colour, LightColour::cyan);
}

TEST(Runtime, MaximumTimeAndSequenceAreHandledWithoutArithmeticOverflow) {
    Runtime runtime{rules};
    constexpr auto max_time = std::numeric_limits<Timestamp>::max();
    constexpr auto max_sequence = std::numeric_limits<SequenceId>::max();
    const std::array events{Event{max_time, max_sequence, ButtonPressed{}}};
    ASSERT_FALSE(runtime.submit_batch(events));
    EXPECT_EQ(runtime.advance_to(max_time).commands,
              (std::vector<CommandRecord>{{max_time, max_sequence, SetLight{LightColour::cyan}}}));
    EXPECT_TRUE(runtime.advance_to(max_time).commands.empty());
}

TEST(Runtime, RepeatedRunsAndSplitHorizonsProduceIdenticalCommands) {
    const std::array events{Event{20, 0, MotionDetected{}}, Event{0, 1, ButtonPressed{}},
                            Event{10, 2, ColourDetected{SensorColour::blue}}};
    Runtime whole{rules};
    ASSERT_FALSE(whole.submit_batch(events));
    const auto expected = whole.advance_to(20).commands;
    for (int run = 0; run < 3; ++run) {
        Runtime split{rules};
        ASSERT_FALSE(split.submit_batch(events));
        std::vector<CommandRecord> actual;
        for (Timestamp horizon : {0, 5, 10, 20}) {
            const auto result = split.advance_to(horizon);
            ASSERT_FALSE(result.error);
            actual.insert(actual.end(), result.commands.begin(), result.commands.end());
        }
        EXPECT_EQ(actual, expected);
    }
}

} // namespace
