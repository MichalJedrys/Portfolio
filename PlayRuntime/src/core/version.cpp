#include "playruntime/version.hpp"
#include "version_config.hpp"

namespace playruntime {

std::string_view version() noexcept { return PLAYRUNTIME_VERSION; }

} // namespace playruntime
