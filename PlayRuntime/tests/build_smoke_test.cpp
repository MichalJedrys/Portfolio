#include "playruntime/version.hpp"

#include <gtest/gtest.h>
#include <string_view>

TEST(BuildSmoke, CoreVersionMatchesConfiguredProject) {
    // This verifies linkage and generated build metadata, not runtime semantics.
    EXPECT_EQ(playruntime::version(), std::string_view{EXPECTED_PROJECT_VERSION});
}
