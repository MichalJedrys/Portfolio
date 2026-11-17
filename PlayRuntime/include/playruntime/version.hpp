#pragma once

#include <string_view>

namespace playruntime {

// Build identity; this is not a runtime capability or session-format version.
[[nodiscard]] std::string_view version() noexcept;

} // namespace playruntime
