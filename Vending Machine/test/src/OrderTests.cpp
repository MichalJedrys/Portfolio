#include <gtest/gtest.h>
#include "VendingMachine.hpp"
#include <sstream>

class VendingOrderTest : public ::testing::Test {
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

    std::string RunOrder(const std::string& script) {
        input.str(script);
        VendingMachine machine;
        machine.work();
        machine.SelectMOption(1); // Inspect transaction history.
        machine.SelectMOption(2); // Inspect collected money.
        return output.str();
    }

    void ExpectAbortedOrder(const std::string& text) {
        EXPECT_NE(text.find("Collected: 0$"), std::string::npos);
        EXPECT_EQ(text.find(" - Coffee"), std::string::npos);
        EXPECT_EQ(text.find(" - Chicken Soup"), std::string::npos);
        EXPECT_EQ(text.find(" is ready."), std::string::npos);
    }

    void ExpectInvalidExtras(const std::string& script) {
        const std::string text = RunOrder(script);
        ExpectAbortedOrder(text);
        EXPECT_NE(text.find("Invalid Operator!"), std::string::npos);
        EXPECT_EQ(text.find("Insert coin"), std::string::npos);
    }

    void ExpectCompletedOrder(const std::string& text, const std::string& receipt,
                              const std::string& total, const std::string& extrasPrompt) {
        const auto receiptPosition = text.find(receipt);
        ASSERT_NE(receiptPosition, std::string::npos);
        EXPECT_EQ(text.find(receipt, receiptPosition + 1), std::string::npos);
        EXPECT_NE(text.find(total), std::string::npos);
        EXPECT_NE(text.find(" is ready."), std::string::npos);
        const auto paymentPosition = text.find("Insert coin");
        ASSERT_NE(paymentPosition, std::string::npos);
        EXPECT_LT(text.find(extrasPrompt), paymentPosition);
    }
};

TEST_F(VendingOrderTest, RejectInvalidMilkBeforePayment) {
    ExpectInvalidExtras("1\nx\nx\nx\n");
}

TEST_F(VendingOrderTest, RejectInvalidSugarBeforePayment) {
    ExpectInvalidExtras("1\ny\n7\nx\nx\n");
}

TEST_F(VendingOrderTest, RejectInvalidSoySauceBeforePayment) {
    ExpectInvalidExtras("6\nx\nx\nx\n");
}

TEST_F(VendingOrderTest, CompleteCoffeeOrderOnce) {
    const std::string text = RunOrder("1\ny\n2\n2\n0.5\nx\nx\n");
    ExpectCompletedOrder(text, "2.5 - Coffee", "Collected: 2.5$", "add milk?");
}

TEST_F(VendingOrderTest, CompleteSoupOrderOnce) {
    const std::string text = RunOrder("6\nn\n1\n2\nx\nx\n");
    ExpectCompletedOrder(text, "3 - Chicken Soup", "Collected: 3$", "add soy sauce?");
}

TEST_F(VendingOrderTest, CancelPaymentWithoutRecordingSale) {
    ExpectAbortedOrder(RunOrder("1\nn\n0\n0\nx\nx\n"));
}
