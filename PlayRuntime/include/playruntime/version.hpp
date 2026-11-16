#pragma once

#include <string_view>

namespace playruntime {

// Build identity only; the runtime engine is not implemented yet.
[[nodiscard]] std::string_view version() noexcept;

} // namespace playruntime
