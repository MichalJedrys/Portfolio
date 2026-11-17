#include "playruntime/runtime.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace playruntime {
namespace {

bool valid(SensorColour colour) {
    switch (colour) {
    case SensorColour::red:
    case SensorColour::green:
    case SensorColour::blue:
    case SensorColour::yellow:
    case SensorColour::white:
    case SensorColour::black:
    case SensorColour::unknown:
        return true;
    }
    return false;
}

bool valid(LightColour colour) {
    switch (colour) {
    case LightColour::red:
    case LightColour::green:
    case LightColour::blue:
    case LightColour::yellow:
    case LightColour::white:
    case LightColour::black:
    case LightColour::unknown:
    case LightColour::off:
    case LightColour::cyan:
        return true;
    }
    return false;
}

bool valid(EventKind kind) {
    switch (kind) {
    case EventKind::button:
    case EventKind::colour:
    case EventKind::motion:
        return true;
    }
    return false;
}

EventKind kind_of(const EventPayload &payload) {
    if (std::holds_alternative<ButtonPressed>(payload)) {
        return EventKind::button;
    }
    if (std::holds_alternative<ColourDetected>(payload)) {
        return EventKind::colour;
    }
    return EventKind::motion;
}

} // namespace

Runtime::Runtime(std::span<const LightRule> rules) : rules_(rules.begin(), rules.end()) {
    for (const auto &rule : rules_) {
        if (!valid(rule.trigger) || !valid(rule.colour)) {
            throw std::invalid_argument("Invalid light rule");
        }
    }
}

std::optional<InputError> Runtime::submit_batch(std::span<const Event> events) {
    auto candidate_sequence = last_sequence_;
    for (std::size_t index = 0; index < events.size(); ++index) {
        const auto &event = events[index];
        if (event.timestamp < 0) {
            return InputError{ErrorCode::negative_time, index};
        }
        if (horizon_ && event.timestamp <= *horizon_) {
            return InputError{ErrorCode::closed_horizon, index};
        }
        if (candidate_sequence && event.sequence <= *candidate_sequence) {
            return InputError{ErrorCode::invalid_sequence, index};
        }
        if (event.payload.valueless_by_exception()) {
            return InputError{ErrorCode::invalid_payload, index};
        }
        if (const auto *colour = std::get_if<ColourDetected>(&event.payload);
            colour && !valid(colour->colour)) {
            return InputError{ErrorCode::invalid_payload, index};
        }
        candidate_sequence = event.sequence;
    }
    if (events.empty()) {
        return std::nullopt;
    }

    // Prepare a replacement before committing state, including if allocation fails.
    auto candidate_queue = pending_;
    candidate_queue.insert(candidate_queue.end(), events.begin(), events.end());
    std::sort(candidate_queue.begin(), candidate_queue.end(), [](const Event &a, const Event &b) {
        return a.timestamp < b.timestamp || (a.timestamp == b.timestamp && a.sequence < b.sequence);
    });
    pending_.swap(candidate_queue);
    last_sequence_ = candidate_sequence;
    return std::nullopt;
}

AdvanceResult Runtime::advance_to(Timestamp horizon) {
    if (horizon < 0) {
        return {{}, InputError{ErrorCode::negative_time, std::nullopt}};
    }
    if (horizon_ && horizon < *horizon_) {
        return {{}, InputError{ErrorCode::closed_horizon, std::nullopt}};
    }
    const auto end =
        std::upper_bound(pending_.begin(), pending_.end(), horizon,
                         [](Timestamp time, const Event &event) { return time < event.timestamp; });
    AdvanceResult result;
    // One command per event at most; allocate before consuming any event.
    result.commands.reserve(static_cast<std::size_t>(end - pending_.begin()));
    for (auto event = pending_.begin(); event != end; ++event) {
        const auto kind = kind_of(event->payload);
        const auto rule =
            std::find_if(rules_.begin(), rules_.end(),
                         [kind](const LightRule &entry) { return entry.trigger == kind; });
        if (rule != rules_.end()) {
            result.commands.push_back({event->timestamp, event->sequence, SetLight{rule->colour}});
        }
    }
    pending_.erase(pending_.begin(), end);
    horizon_ = horizon;
    return result;
}

std::optional<Timestamp> Runtime::closed_horizon() const noexcept { return horizon_; }

std::size_t Runtime::pending_events() const noexcept { return pending_.size(); }

} // namespace playruntime
