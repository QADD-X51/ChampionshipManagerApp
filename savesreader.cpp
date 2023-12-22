#include "savesreader.h"

#include <stdio.h>

SavesReader::SavesReader() {}

void SavesReader::ReadSaves()
{

    for(const auto& file : std::filesystem::directory_iterator(std::filesystem::current_path() / "saves"))
    {
        if(file.path().extension().string() != ".qad")
        {
            continue;
        }

        std::ifstream in(file.path());

        std::string name;
        unsigned int nr_events;
        unsigned int nr_conc;

        std::getline(in, name);
        in>>nr_events;
        in>>nr_conc;

        in.close();

        Save save(file.path().string(), name, nr_events, nr_conc);

        this->saves.push_back(save);

    }
}

void SavesReader::DeleteSaves()
{
    this->saves.clear();
}

std::vector<Save> SavesReader::GetSaves() const
{
    return saves;
}

void SavesReader::DeleteSaveFile(const unsigned int& index) const
{
    remove(this->saves.at(index).GetPath().c_str());
}

std::string SavesReader::GetSaveName(const unsigned int& index) const
{
    return this->saves.at(index).GetName();
}

unsigned int SavesReader::GetSavesCount() const
{
    return this->saves.size();
}

std::string SavesReader::GetPathOfSave(const unsigned int & index) const
{
    return this->saves.at(index).GetPath();
}
