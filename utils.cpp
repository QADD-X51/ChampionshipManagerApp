#include "utils.h"

std::string utils::IntToPositionString(const unsigned int& number)
{
    if(number%100 == 11 || number % 100 == 12 || number % 100 == 13)
    {
        return std::to_string(number) + "th";
    }

    if(number % 10 == 1)
    {
        return std::to_string(number) + "st";
    }

    if(number % 10 == 2)
    {
        return std::to_string(number) + "nd";
    }

    if(number % 10 == 1)
    {
        return std::to_string(number) + "rd";
    }

    return std::to_string(number) + "th";
}

unsigned int utils::PickRandomElementFromArray(const std::vector<unsigned int>& array)
{
    if(array.size() == 1)
    {
        return array.at(0);
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,array.size() - 1);

    return array.at(dist(rng));
}

unsigned int utils::GenerateRandomUnsignedInt(const unsigned int& start, const unsigned int& stop)
{
    if(stop <= start) return 0;
    if(stop - start == 1) return start;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(start, stop - 1);

    return dist(rng);
}

std::vector<unsigned int> utils::MakeArray(const unsigned int& start, const unsigned int& stop)
{
    std::vector<unsigned int> array;
    for(unsigned int index=start;index<stop;++index)
    {
        array.push_back(index);
    }
    return array;
}

/*
std::string utils::GetRandomName()
{
    unsigned int rando = utils::GenerateRandomUnsignedInt(0, utils::randomNames.size());
    return utils::randomNames.at(rando);
}
*/
