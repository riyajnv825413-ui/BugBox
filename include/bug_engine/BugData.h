#ifndef BUGDATA_H
#define BUGDATA_H

#include <string>

enum class Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

struct BugData
{
    std::string bugType;
    Difficulty difficulty;

    std::string buggyCode;
    std::string expectedOutput;

    std::string hint;
    std::string solution;
};

#endif