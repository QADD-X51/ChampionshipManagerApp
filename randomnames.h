#ifndef RANDOMNAMES_H
#define RANDOMNAMES_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

class RandomNames
{
public:
    RandomNames();
    std::string GetRandomName() const;
    bool GetFileOpened() const;
private:
    std::vector<std::string> randomNames;
    bool randomNamesFileOpened;
};

#endif // RANDOMNAMES_H
