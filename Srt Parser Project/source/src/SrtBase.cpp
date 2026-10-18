#include "SrtBase.hpp"

#include <limits>

bool TimeLineCheck(int i, char c)
{
	std::array<int,18> Arr = { 1,2,4,5,7,8,10,11,12,16,17,19,20,22,23,25,26,27 };
	if (std::any_of(Arr.begin(), Arr.end(), [i](int x) { return i == x; }) && !isdigit(c))
		return false;
	if ((i == 3 || i == 6 || i == 18 || i == 21) && c != ':')
		return false;
	if ((i == 9 || i == 24) && c != ',')
		return false;
	if ((i == 13 || i == 14) && c != '-')
		return false;
	if (i == 15 && c != '>')
		return false;
	return true;
}

pairVec SrtParserBase::Arrange()
{
    vec.clear();
    std::vector<std::string>lines;
    std::string newLine = "";
    for (auto& s : data)
    {
        newLine = newLine + s;
        if (s == '\n')
        {
            lines.push_back(newLine);
            newLine = "";
        }
    }

    if (!newLine.empty())
        lines.push_back(newLine + '\n');
    if (!lines.empty() && lines.back() != "\n")
        lines.push_back("\n");

    struct Part
    {
        double stTime = 0.0;
        double enTime = 0.0;
        std::string s_text = "";
    };

    std::vector<Part>parts;
    bool startToEnd = 0;
    bool text = false;
    bool time = false;
    double arrTime[] = { 36000,3600,600,60,10,1,0.1,0.01,0.001 };
    int arrI = 0;
    Part tempPart;
    int checkNum = 0, checkNumPrev = 0;
    bool endPart = false;
    int timeC = 1;
    bool badTime = false;

    for (auto &line : lines)
    {
        if (text)
        {
            tempPart.s_text = tempPart.s_text + line;
        }

        if (time)
        {
            timeC = 1;
            for (auto c : line)
            {
                if (c != ' ')
                {
                    if (!TimeLineCheck(timeC, c))
                    {
                        badTime = true;
                        time = false;
                    }
                    timeC++;
                } 
                
            }
            if (timeC != 29)
                badTime = true;

            if (!badTime)
            {
                for (auto c : line)
                {
                    if (arrI == 9 && startToEnd == false)
                    {
                        startToEnd = true;
                        arrI = 0;
                    }

                    if (arrI == 9 && startToEnd == true)
                    {
                        startToEnd = false;
                        arrI = 0;
                    }

                    if (isdigit(c) && startToEnd == false)
                    {
                        tempPart.stTime = tempPart.stTime + (c - '0') * arrTime[arrI];
                        arrI++;
                    }

                    if (isdigit(c) && startToEnd == true)
                    {
                        tempPart.enTime = tempPart.enTime + (c - '0') * arrTime[arrI];
                        arrI++;
                    }

                    if (c == '\n')
                    {
                        time = false;
                        text = true;
                    }
                }
            }
        }

        if (!time && !text && !endPart)
        {
            endPart = 1;
            checkNum = 0;
            for (auto c : line)
            {
                if (c >= '0' && c <= '9')
                {
                    const int digit = c - '0';
                    if (checkNum > (std::numeric_limits<int>::max() - digit) / 10)
                    {
                        checkNum = checkNumPrev;
                        break;
                    }
                    checkNum = checkNum * 10 + digit;
                }
                else if (c != '\n')
                {
                    checkNum = checkNumPrev;
                    break;
                }
            }
            if (checkNumPrev == checkNum - 1)
            {
                time = true;
                checkNumPrev = checkNum;
            }
        }

        if (line == "\n" && endPart)
        {
            text = false;
            parts.push_back(tempPart);
            tempPart.stTime = 0.0;
            tempPart.enTime = 0.0;
            tempPart.s_text = "";
            endPart = 0;
            badTime = false;
        }
    }

    double end_Time_val = 0.0;
    std::pair<double, std::string> pos;
    for (auto p : parts)
    {
        if (p.enTime > end_Time_val)
            end_Time_val = p.enTime;
    }
    double currTime = 0;
    std::vector<double>dbVec;
    const double timeStep = 0.1;

    while (currTime <= end_Time_val+timeStep/2)
    {
        dbVec.push_back(currTime);
        currTime += timeStep;
    }

    for (auto t : dbVec)
    {
        for (auto p : parts)
        {
            if (t >= p.stTime - timeStep/2 && t <= p.enTime - timeStep/2)
            {
                pos.second = pos.second + p.s_text;
            }
        }
        pos.first = t;
        vec.push_back(pos);
        pos.first = 0;
        pos.second = "";
    }

    std::string prevD="";
    for (auto &d : vec)
    {
        if (d.second == prevD)
        {
            prevD = d.second;
            d.second = "delete";
        }
        else 
        {
            prevD = d.second;
        }
    }

    vec.erase(remove_if(vec.begin(), vec.end(), [](std::pair<double, std::string> p) {return p.second == "delete"; }), vec.end());
    
    char prevC = 'a';
    for (auto& d : vec)
    {
        for (int i = 0; i < d.second.length(); i++)
        {
            if (d.second[i] == '\n' && i == d.second.length() - 1)
            {
                d.second.erase(d.second.begin() + i);
            }

            if(d.second[i] == '\n' && prevC == '\n')
                d.second.erase(d.second.begin() + i);

            prevC = d.second[i];
        }
        if (d.second.length()>2)
            d.second.erase(d.second.end() - 1);
    }

    return vec;
}
