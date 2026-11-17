#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <variant>
#include <vector>

namespace playruntime {

// Nonnegative logical microseconds; never obtained from a wall clock by the core.
using Timestamp = std::int64_t;
using SequenceId = std::uint64_t;

enum class SensorColour { red, green, blue, yellow, white, black, unknown };
enum class LightColour { red, green, blue, yellow, white, black, unknown, off, cyan };
enum class EventKind { button, colour, motion };

struct ButtonPressed {
    bool operator==(const ButtonPressed &) const = default;
};
struct ColourDetected {
    SensorColour colour;
    bool operator==(const ColourDetected &) const = default;
};
struct MotionDetected {
    bool operator==(const MotionDetected &) const = default;
};
using EventPayload = std::variant<ButtonPressed, ColourDetected, MotionDetected>;

struct Event {
    Timestamp timestamp;
    SequenceId sequence;
    EventPayload payload;
    bool operator==(const Event &) const = default;
};

struct SetLight {
    LightColour colour;
    bool operator==(const SetLight &) const = default;
};
struct CommandRecord {
    Timestamp timestamp;
    SequenceId source_sequence;
    SetLight command;
    bool operator==(const CommandRecord &) const = default;
};

// Minimal programmatic rule: the first matching event kind emits one light command.
// This is not the future state-machine or scripting representation.
struct LightRule {
    EventKind trigger;
    LightColour colour;
};

enum class ErrorCode { negative_time, closed_horizon, invalid_sequence, invalid_payload };
struct InputError {
    ErrorCode code;
    // Present for a rejected batch element, absent for an invalid advance horizon.
    std::optional<std::size_t> event_index;
    bool operator==(const InputError &) const = default;
};
struct AdvanceResult {
    // Host-owned output; the runtime retains no command history.
    std::vector<CommandRecord> commands;
    std::optional<InputError> error;
};

class Runtime {
  public:
    // Copies the rules. Invalid enum values throw std::invalid_argument.
    explicit Runtime(std::span<const LightRule> rules);

    // All-or-nothing validation and insertion. Sequence IDs strictly increase in
    // submitted order across batches, even when timestamps arrive out of order.
    [[nodiscard]] std::optional<InputError> submit_batch(std::span<const Event> events);

    // Inclusive horizon. Equal horizons are no-ops; decreasing/negative time fails.
    [[nodiscard]] AdvanceResult advance_to(Timestamp horizon);
    [[nodiscard]] std::optional<Timestamp> closed_horizon() const noexcept;
    [[nodiscard]] std::size_t pending_events() const noexcept;

  private:
    std::vector<LightRule> rules_;
    std::vector<Event> pending_;
    std::optional<Timestamp> horizon_;
    std::optional<SequenceId> last_sequence_;
};

} // namespace playruntime
