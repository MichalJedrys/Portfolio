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
        std::ostringstream timestamp;
        timestamp.imbue(std::locale::classic());
        timestamp << std::fixed << std::setprecision(2) << p.first;
        std::string time = timestamp.str();
        std::replace(time.begin(), time.end(), '.', ',');
        toString << "at " << time << " show '" << p.second << "'\n";
    }
    filename = toString.str();

    return filename;
}