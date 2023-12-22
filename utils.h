#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <random>
#include <filesystem>
#include <fstream>

struct utils
{

    static std::string IntToPositionString(const unsigned int& number);

    static unsigned int PickRandomElementFromArray(const std::vector<unsigned int>& array);

    static unsigned int GenerateRandomUnsignedInt(const unsigned int& start, const unsigned int& stop);

    static std::vector<unsigned int> MakeArray(const unsigned int& start, const unsigned int& stop);

    static const char endOfNoteChar = 178;

private:
};

#endif // UTILS_H
