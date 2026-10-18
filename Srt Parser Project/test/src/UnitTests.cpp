#include <gtest/gtest.h>
#include "SrtParserFakeFile.hpp"
#include "SrtParserRealFile.hpp"
#include <stdexcept>

class SrtParserTests : public ::testing::Test {
protected:
    SrtParserFakeFile SP_fake;
    
	std::string fillA(int i)
	{
		std::string out;
		for (int j = 0; j <= i%10; j++)
		{
			out = out + "A";
		}
		return out;
	}

    void SetUp() override {
    }
    
    void TearDown() override {
    }
};

TEST_F(SrtParserTests, ExampleFile) {

	const std::string TestedInput = "1\n00:00 : 01, 000 -- > 00:00 : 05, 500\nRaz\ndva\ntri\n\n2\n00:00 : 06, 000 -- > 00:00 : 09, 500\nchetyre piat\n\n3\n00:00 : 10, 000 -- > 00:00 : 15, 500\nvyshel\nzaichiq\npoguliat\n\n4\n00:00 : 11, 000 -- > 00:00 : 19, 500\nbelyi zaichiq\n\n5\n00 : 00 : 12, 000 -- > 00:00 : 14, 500\nnaglyi zaichiq\n\n6\n00 : 00 : 13, 000 -- > 00:00 : 16, 500\nvyshel\ni ushel\n\n7\n00 : 00 : 15, 000 -- > 00:00 : 17, 500\ntoptun\n\n8\n00 : 00 : 19, 500 -- > 00:00 : 19, 600\nvot i vsyo\n\n";
	const std::string TestedOutput = "at 1,00 show 'Raz\ndva\ntri'\nat 5,50 show ''\nat 6,00 show 'chetyre piat'\nat 9,50 show ''\nat 10,00 show 'vyshel\nzaichiq\npoguliat'\nat 11,00 show 'vyshel\nzaichiq\npoguliat\nbelyi zaichiq'\nat 12,00 show 'vyshel\nzaichiq\npoguliat\nbelyi zaichiq\nnaglyi zaichiq'\nat 13,00 show 'vyshel\nzaichiq\npoguliat\nbelyi zaichiq\nnaglyi zaichiq\nvyshel\ni ushel'\nat 14,50 show 'vyshel\nzaichiq\npoguliat\nbelyi zaichiq\nvyshel\ni ushel'\nat 15,00 show 'vyshel\nzaichiq\npoguliat\nbelyi zaichiq\nvyshel\ni ushel\ntoptun'\nat 15,50 show 'belyi zaichiq\nvyshel\ni ushel\ntoptun'\nat 16,50 show 'belyi zaichiq\ntoptun'\nat 17,50 show 'belyi zaichiq'\nat 19,50 show 'vot i vsyo'\nat 19,60 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output,TestedOutput);
}

TEST_F(SrtParserTests, SimpleCase)
{
	const std::string TestedInput = "1\n00:00 : 01, 000 -- > 00:00 : 05, 500\nRaz\ndva\ntri\n\n2\n00:00 : 06, 000 -- > 00:00 : 09, 500\nchetyre piat\n\n3\n00:00 : 10, 000 -- > 00:00 : 12, 500\nvyshel\nzaichiq\npoguliat\n\n";
	const std::string TestedOutput = "at 1,00 show 'Raz\ndva\ntri'\nat 5,50 show ''\nat 6,00 show 'chetyre piat'\nat 9,50 show ''\nat 10,00 show 'vyshel\nzaichiq\npoguliat'\nat 12,50 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}


TEST_F(SrtParserTests, FinalCueWithoutTrailingNewline)
{
	const std::string TestedInput = "1\n00:00 : 01, 000 -- > 00:00 : 05, 500\nRaz\ndva\ntri\n\n2\n00:00 : 06, 000 -- > 00:00 : 09, 500\nchetyre piat\n\n3\n00:00 : 10, 000 -- > 00:00 : 12, 500\nvyshel\nzaichiq\npoguliat";
	const std::string TestedOutput = "at 1,00 show 'Raz\ndva\ntri'\nat 5,50 show ''\nat 6,00 show 'chetyre piat'\nat 9,50 show ''\nat 10,00 show 'vyshel\nzaichiq\npoguliat'\nat 12,50 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}


TEST_F(SrtParserTests, IncorrectRecordNumbers)
{
	const std::string TestedInput = "1\n00:00 : 01, 000 -- > 00:00 : 05, 500\nRaz\ndva\ntri\n\n2dD\n00:00 : 06, 000 -- > 00:00 : 09, 500\nchetyre piat\n\n2\n00:00 : 10, 000 -- > 00:00 : 12, 500\nvyshel\nzaichiq\npoguliat\n\n";
	const std::string TestedOutput = "at 1,00 show 'Raz\ndva\ntri'\nat 5,50 show ''\nat 10,00 show 'vyshel\nzaichiq\npoguliat'\nat 12,50 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, Nesting)
{
	const std::string TestedInput = "1\n00:00 : 02, 000 -- > 00:00 : 12, 000\nA\nA\nA\n\n2\n00:00 : 04, 000 -- > 00:00 : 10, 000\nB\nB\nB\n\n3\n00:00 : 06, 000 -- > 00:00 : 08, 000\nC\nC\nC\n\n";
	const std::string TestedOutput = "at 2,00 show 'A\nA\nA'\nat 4,00 show 'A\nA\nA\nB\nB\nB'\nat 6,00 show 'A\nA\nA\nB\nB\nB\nC\nC\nC'\nat 8,00 show 'A\nA\nA\nB\nB\nB'\nat 10,00 show 'A\nA\nA'\nat 12,00 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, Empty)
{
	const std::string TestedInput = "";
	const std::string TestedOutput = "";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, Intersection)
{
	const std::string TestedInput = "1\n00:00 : 02, 000 -- > 00:00 : 08, 000\nA\nA\nA\n\n2\n00:00 : 04, 000 -- > 00:00 : 10, 000\nB\nB\nB\n\n3\n00:00 : 06, 000 -- > 00:00 : 12, 000\nC\nC\nC\n\n";
	const std::string TestedOutput = "at 2,00 show 'A\nA\nA'\nat 4,00 show 'A\nA\nA\nB\nB\nB'\nat 6,00 show 'A\nA\nA\nB\nB\nB\nC\nC\nC'\nat 8,00 show 'B\nB\nB\nC\nC\nC'\nat 10,00 show 'C\nC\nC'\nat 12,00 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, ManySequentialRecords)
{
	int N = 5000;
	std::string TestedInput;
	std::string strA;
	for (int i = 1; i <= N; i++)
	{
		strA = fillA(i);
		TestedInput = TestedInput + std::to_string(i) + "\n00:00 : 01,000 -- > 00:00 : 10,000\nThe code is " + strA + "!\n\n";
	}

	std::string TestedOutput = "at 1,00 show '";
	for (int i = 1; i <= N; i++)
	{
		strA = fillA(i);
		TestedOutput = TestedOutput + "The code is " + strA + "!\n";
	}
	TestedOutput.pop_back();
	TestedOutput = TestedOutput + "'\nat 10,00 show ''\n";

	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, WrongTimes)
{
	const std::string TestedInput = "1\n00:00 : 01, 000 -- > 00:00 : 05, 500\nRaz\ndva\ntri\n\n2\n00:AA : 06, 000 -- > 00:00 : 09, 500\nchetyre piat\n\n3\n00:00 : 10, 000 -- > 00:00 : 999, 500\nvyshel\nzaichiq\npoguliat\n\n3\n00:00 : 12, 00DD0 -> 00:00 : 15, 500\nvyshel\nzaichiq\npoguliat\n\n";
	const std::string TestedOutput = "at 1,00 show 'Raz\ndva\ntri'\nat 5,50 show ''\n";
	std::string Output;

	std::istringstream istr(TestedInput);

	SP_fake.ReadFile(istr);

	SP_fake.Arrange();

	Output = SP_fake.WriteToFile(Output);

	EXPECT_EQ(Output, TestedOutput);
}

TEST_F(SrtParserTests, FinalCueWithSingleTrailingNewline)
{
    std::istringstream input("1\n00:00:01,000 --> 00:00:02,000\nHello\n");
    SP_fake.ReadFile(input);
    SP_fake.Arrange();
    EXPECT_EQ(SP_fake.WriteToFile(""), "at 1,00 show 'Hello'\nat 2,00 show ''\n");
}

TEST_F(SrtParserTests, RepeatedArrangeProducesSameEvents)
{
    std::istringstream input("1\n00:00:01,000 --> 00:00:02,000\nHello\n\n");
    SP_fake.ReadFile(input);
    const auto first = SP_fake.Arrange();
    EXPECT_EQ(SP_fake.Arrange(), first);
    EXPECT_EQ(SP_fake.WriteToFile(""), "at 1,00 show 'Hello'\nat 2,00 show ''\n");
}

TEST_F(SrtParserTests, ReadingNewInputClearsPreviousEvents)
{
    std::istringstream first("1\n00:00:01,000 --> 00:00:02,000\nOld\n\n");
    SP_fake.ReadFile(first);
    SP_fake.Arrange();

    std::istringstream second("1\n00:00:03,000 --> 00:00:04,000\nNew\n\n");
    SP_fake.ReadFile(second);
    EXPECT_TRUE(SP_fake.WriteToFile("").empty());
    SP_fake.Arrange();
    EXPECT_EQ(SP_fake.WriteToFile(""), "at 3,00 show 'New'\nat 4,00 show ''\n");

    std::istringstream empty("");
    SP_fake.ReadFile(empty);
    EXPECT_TRUE(SP_fake.WriteToFile("").empty());
    EXPECT_TRUE(SP_fake.Arrange().empty());
}

TEST(SrtParserRealFileTests, ReadingNewInputReplacesPreviousEvents)
{
    SrtParserRealFile parser;
    std::istringstream first("1\n00:00:01,000 --> 00:00:02,000\nOld\n\n");
    parser.ReadFile(first);
    parser.Arrange();

    std::istringstream second("1\n00:00:03,000 --> 00:00:04,000\nNew\n\n");
    parser.ReadFile(second);
    const auto events = parser.Arrange();
    ASSERT_EQ(events.size(), 2u);
    EXPECT_NEAR(events[0].first, 3.0, 1e-9);
    EXPECT_EQ(events[0].second, "New");
    EXPECT_NEAR(events[1].first, 4.0, 1e-9);
    EXPECT_TRUE(events[1].second.empty());

    std::istringstream empty("");
    parser.ReadFile(empty);
    EXPECT_TRUE(parser.Arrange().empty());
}

TEST_F(SrtParserTests, SubtitleTextIsPreserved)
{
    for (const std::string text : {"A", "AB", "delete", "Hello.", "It's 3.14.",
                                  "  Keep spaces  ", "Line one\nLine two"})
    {
        SCOPED_TRACE(text);
        std::istringstream input("1\n00:00:01,000 --> 00:00:02,000\n" + text + "\n\n");
        SP_fake.ReadFile(input);
        SP_fake.Arrange();
        EXPECT_EQ(SP_fake.WriteToFile(""), "at 1,00 show '" + text + "'\nat 2,00 show ''\n");
    }
}

TEST_F(SrtParserTests, OverlappingSubtitlesPreserveTextAndLineBreaks)
{
    std::istringstream input("1\n00:00:01,000 --> 00:00:03,000\nA\nB\n\n"
                             "2\n00:00:02,000 --> 00:00:04,000\nC.\n\n");
    SP_fake.ReadFile(input);
    SP_fake.Arrange();
    EXPECT_EQ(SP_fake.WriteToFile(""),
              "at 1,00 show 'A\nB'\nat 2,00 show 'A\nB\nC.'\nat 3,00 show 'C.'\nat 4,00 show ''\n");
}

TEST(SrtParserRealFileTests, UnopenedInputThrows)
{
    SrtParserRealFile parser;
    std::ifstream input("");
    EXPECT_THROW(parser.ReadFile(input), std::runtime_error);
}

TEST(SrtParserRealFileTests, FailedInputThrows)
{
    SrtParserRealFile parser;
    std::istringstream input("content");
    input.setstate(std::ios::badbit);
    EXPECT_THROW(parser.ReadFile(input), std::runtime_error);
}

TEST(SrtParserRealFileTests, ErrorDuringReadThrows)
{
    class FailingBuffer : public std::streambuf
    {
        std::streamsize xsgetn(char*, std::streamsize) override
        {
            throw std::runtime_error("Simulated read error");
        }
    } buffer;
    std::istream input(&buffer);
    SrtParserRealFile parser;
    EXPECT_THROW(parser.ReadFile(input), std::runtime_error);
}

TEST(SrtParserRealFileTests, EmptyInputIsValid)
{
    SrtParserRealFile parser;
    std::istringstream input("");
    EXPECT_NO_THROW(parser.ReadFile(input));
    EXPECT_TRUE(parser.Arrange().empty());
}

TEST(SrtParserRealFileTests, InvalidOutputPathThrows)
{
    SrtParserRealFile parser;
    EXPECT_THROW(parser.WriteToFile(""), std::runtime_error);
}
