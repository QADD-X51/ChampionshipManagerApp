#include "randomnames.h"

#include "utils.h"

RandomNames::RandomNames()
{
    auto newFilePath = std::filesystem::current_path() / "resources" / "names.txt";

    if(!std::filesystem::exists(newFilePath))
    {
        this->randomNamesFileOpened = false;
        return;
    }

    this->randomNamesFileOpened = true;

    std::ifstream inFile(newFilePath);

    std::string name;

    while(std::getline(inFile, name))
    {
        if(name == "" || name.find_first_not_of(' ') == std::string::npos)
        {
            continue;
        }
        this->randomNames.push_back(name);
    }
}

std::string RandomNames::GetRandomName() const
{
    unsigned int rando = utils::GenerateRandomUnsignedInt(0, this->randomNames.size());
    return this->randomNames.at(rando);
}

bool RandomNames::GetFileOpened() const
{
    return this->randomNamesFileOpened;
}
