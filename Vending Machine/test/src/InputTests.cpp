#include <gtest/gtest.h>
#include "VendingMachine.hpp"
#include <sstream>

struct InputCase {
    const char* name;
    const char* script;
    bool eof = false;
};

class VendingInputTest : public ::testing::TestWithParam<InputCase> {
protected:
    std::istringstream input;
    std::ostringstream output;
    std::streambuf* previousInput = nullptr;
    std::streambuf* previousOutput = nullptr;

    void SetUp() override {
        previousInput = std::cin.rdbuf(input.rdbuf());
        previousOutput = std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        std::cin.rdbuf(previousInput);
        std::cout.rdbuf(previousOutput);
    }
};

TEST_P(VendingInputTest, HandleInputWithoutCompletingOrder) {
    const auto& test = GetParam();
    input.str(std::string(test.script) + (test.eof ? "" : "5\n"));
    VendingMachine machine;
    machine.work();
    if (test.eof) {
        EXPECT_TRUE(std::cin.eof());
        EXPECT_EQ(output.str().find("Error:"), std::string::npos);
    } else {
        ASSERT_TRUE(std::cin.good());
        if (std::string(test.name).find("Main") == 0)
            EXPECT_EQ(output.str().find("add milk?"), std::string::npos);
        EXPECT_EQ(machine.MainMenu(), 5); // The next prompt must consume a fresh answer.
    }
    EXPECT_EQ(output.str().find(" is ready."), std::string::npos);
    EXPECT_EQ(output.str().find("Insert coin"), std::string::npos);
}

INSTANTIATE_TEST_SUITE_P(Console, VendingInputTest, ::testing::Values(
    InputCase{"MainText", "abc\n"},
    InputCase{"MainFraction", "1.5\n"},
    InputCase{"MainTrailingText", "1x\n"},
    InputCase{"MainOverflow", "9999999999999999999999\n"},
    InputCase{"MainBlank", "\n"},
    InputCase{"MainOutOfRange", "12\n"},
    InputCase{"MilkTrailingText", "1\nyes\n"},
    InputCase{"MilkBlank", "1\n\n"},
    InputCase{"SugarText", "1\nn\nabc\n"},
    InputCase{"SugarFraction", "2\n1.5\n"},
    InputCase{"SugarOutOfRange", "2\n7\n"},
    InputCase{"SoyTrailingText", "6\nyes\n"},
    InputCase{"MaintenanceText", "11\nHardPassword\nabc\n"},
    InputCase{"MaintenanceFraction", "11\nHardPassword\n1.5\n"},
    InputCase{"MaintenanceOutOfRange", "11\nHardPassword\n5\n"},
    InputCase{"WrongPassword", "11\nwrong password\n"},
    InputCase{"MainEof", "", true},
    InputCase{"MilkEof", "1\n", true},
    InputCase{"SugarEof", "1\nn\n", true},
    InputCase{"SoyEof", "6\n", true},
    InputCase{"PasswordEof", "11\n", true},
    InputCase{"MaintenanceEof", "11\nHardPassword\n", true},
    InputCase{"HistoryPauseEof", "11\nHardPassword\n1\n", true},
    InputCase{"WithdrawalPauseEof", "11\nHardPassword\n2\n", true}
), [](const ::testing::TestParamInfo<InputCase>& info) { return info.param.name; });
