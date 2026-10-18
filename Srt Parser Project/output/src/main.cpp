#include "SrtParserRealFile.hpp"
#include <stdexcept>
#include <filesystem>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "--help")
	{
		std::cout << "Usage: SrtParser [input.srt output.txt]\n"
		          << "Without arguments, read TestInput.srt and write Output.txt.\n";
		return 0;
	}
	if (argc != 1 && argc != 3)
	{
		std::cerr << "Usage: SrtParser [input.srt output.txt]\n";
		return 2;
	}
	const std::string inputPath = argc == 3 ? argv[1] : "TestInput.srt";
	const std::string outputPath = argc == 3 ? argv[2] : "Output.txt";

	try
	{
		SrtParserRealFile SP;

		//Import data form file
		ifstream InputFile;
		InputFile.open(inputPath);
		if (!InputFile)
			throw std::runtime_error("Cannot open input file: " + inputPath);
		std::error_code pathError;
		if (std::filesystem::equivalent(inputPath, outputPath, pathError))
			throw std::runtime_error("Input and output paths must differ");

		//Read file
		SP.ReadFile(InputFile);

		InputFile.close();

		//Change format of data
		SP.Arrange();

		//Save to output file
		SP.WriteToFile(outputPath);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}