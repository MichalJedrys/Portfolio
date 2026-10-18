#include "SrtParserRealFile.hpp"
#include <stdexcept>

using namespace std;

int main()
{
	try
	{
		SrtParserRealFile SP;

		//Import data form file
		ifstream InputFile;
		InputFile.open("TestInput.srt");
		if (!InputFile)
			throw std::runtime_error("Cannot open input file: TestInput.srt");

		//Read file
		SP.ReadFile(InputFile);

		InputFile.close();

		//Change format of data
		SP.Arrange();

		//Save to output file
		SP.WriteToFile("Output.txt");
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}