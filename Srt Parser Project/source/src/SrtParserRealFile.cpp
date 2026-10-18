#include "SrtParserRealFile.hpp"

void SrtParserRealFile::ReadFile(std::istream& input)
{
    std::string data_in(std::istreambuf_iterator<char>(input), {});
    data = data_in;
    vec.clear();
}

std::string SrtParserRealFile::WriteToFile(std::string filename)
{
    std::ofstream OutputFile;
    OutputFile.open(filename, std::ios::out | std::ios::trunc);
    OutputFile.imbue(std::locale("Polish_Poland"));

    for (auto &p : vec)
    {
        OutputFile << "at " << std::fixed << std::setprecision(2) << p.first << " show '" << p.second << "'\n";
    }
    OutputFile.close();

    return "";
}