#include <gtest/gtest.h>
#include "Controller.hpp"
#include <sstream>

class VendingPaymentTest : public ::testing::Test {
protected:
    Controller controller;
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

    void RunPayment(const std::string& script, bool completed, bool eof = false) {
        input.str(script + (eof ? "" : "x\nx\n"));
        EXPECT_EQ(controller.PaymentProcessing(controller.SelectBev(1)), completed);
        controller.SelectMOption(1); // Inspect transaction history.
        controller.SelectMOption(2); // Inspect collected money.
        const std::string text = output.str();
        const auto receipt = text.find("2.5 - Coffee");
        if (completed) {
            ASSERT_NE(receipt, std::string::npos);
            EXPECT_EQ(text.find("2.5 - Coffee", receipt + 1), std::string::npos);
            EXPECT_NE(text.find("Collected: 2.5$"), std::string::npos);
            EXPECT_EQ(text.find("Refunding"), std::string::npos);
        } else {
            EXPECT_EQ(receipt, std::string::npos);
            EXPECT_NE(text.find("Collected: 0$"), std::string::npos);
            EXPECT_NE(text.find("Payment cancelled."), std::string::npos);
        }
    }

    void ExpectInvalidCoin() {
        EXPECT_NE(output.str().find("Machine does not accept this currency!"), std::string::npos);
    }
};

TEST_F(VendingPaymentTest, CancelBeforeInsertingCoins) {
    RunPayment("0\n", false);
    EXPECT_EQ(output.str().find("Refunding"), std::string::npos);
}

TEST_F(VendingPaymentTest, RefundAllAcceptedCoinsOnCancellation) {
    RunPayment("1\n0.5\n0\n", false);
    EXPECT_NE(output.str().find("Refunding 1.5$"), std::string::npos);
}

TEST_F(VendingPaymentTest, RetryAfterInvalidFirstCoin) {
    RunPayment("3\n2\n0.5\n", true);
    ExpectInvalidCoin();
    EXPECT_EQ(output.str().find("Your change is"), std::string::npos);
}

TEST_F(VendingPaymentTest, PreserveCreditAfterInvalidCoin) {
    RunPayment("1\n3\n1\n0.5\n", true);
    ExpectInvalidCoin();
    EXPECT_EQ(output.str().find("Your change is"), std::string::npos);
}

TEST_F(VendingPaymentTest, RejectNegativeCoinWithoutChangingRefund) {
    RunPayment("1\n-1\n0\n", false);
    ExpectInvalidCoin();
    EXPECT_NE(output.str().find("Refunding 1$"), std::string::npos);
}

TEST_F(VendingPaymentTest, RetryAfterTextInputWithoutLosingCredit) {
    RunPayment("1\ninvalid\n1\n0.5\n", true);
    ExpectInvalidCoin();
    EXPECT_EQ(output.str().find("Your change is"), std::string::npos);
}

TEST_F(VendingPaymentTest, RefundAcceptedCoinsOnEndOfInput) {
    RunPayment("1\n0.5\n", false, true);
    EXPECT_NE(output.str().find("Refunding 1.5$"), std::string::npos);
}

TEST_F(VendingPaymentTest, RejectTrailingTextWithoutAcceptingCoin) {
    RunPayment("1\n1junk\n1\n0.5\n", true);
    ExpectInvalidCoin();
    EXPECT_EQ(output.str().find("Your change is"), std::string::npos);
}

TEST_F(VendingPaymentTest, ReturnChangeAfterOverpayment) {
    RunPayment("5\n", true);
    EXPECT_NE(output.str().find("Your change is 2.5$"), std::string::npos);
}
