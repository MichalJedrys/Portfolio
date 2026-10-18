#include "SrtParserRealFile.hpp"

#include <stdexcept>

namespace
{
class CommaDecimal : public std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};
}

void SrtParserRealFile::ReadFile(std::istream& input)
{
    if (!input)
        throw std::runtime_error("Cannot read input stream");

    std::string data_in;
    char buffer[4096];
    while (input.read(buffer, sizeof buffer) || input.gcount() > 0)
        data_in.append(buffer, static_cast<std::size_t>(input.gcount()));
    if (input.bad() || !input.eof())
        throw std::runtime_error("Failed to read input stream");
    data = data_in;
    vec.clear();
}

std::string SrtParserRealFile::WriteToFile(std::string filename)
{
    std::ofstream OutputFile;
    OutputFile.open(filename, std::ios::out | std::ios::trunc);
    if (!OutputFile)
        throw std::runtime_error("Cannot open output file: " + filename);
    OutputFile.imbue(std::locale(std::locale::classic(), new CommaDecimal));

    for (auto &p : vec)
    {
        OutputFile << "at " << std::fixed << std::setprecision(2) << p.first << " show '" << p.second << "'\n";
    }
    OutputFile.close();
    if (!OutputFile)
        throw std::runtime_error("Failed to write output file: " + filename);

    return "";
}