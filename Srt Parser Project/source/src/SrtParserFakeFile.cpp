#include "SrtParserFakeFile.hpp"

void SrtParserFakeFile::ReadFile(std::istream& input)
{
    std::string data_in(std::istreambuf_iterator<char>(input), {});
    data = data_in;
    vec.clear();
}

std::string SrtParserFakeFile::WriteToFile(std::string filename)
{
    std::ostringstream toString;
    for (auto& p : vec)
    {
        toString << "at " << std::fixed << std::setprecision(2) << p.first << " show '" << p.second << "'\n";
    }
    filename = toString.str();

    for (auto &c : filename)
    {
        if (c == '.')
            c = ',';
    }

    return filename;
}