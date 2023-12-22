#ifndef SAVESREADER_H
#define SAVESREADER_H

#include <vector>
#include <fstream>
#include <filesystem>

#include "save.h"

class SavesReader
{
private:
    std::vector<Save> saves;

public:
    SavesReader();

    void ReadSaves();
    void DeleteSaves();
    std::vector<Save> GetSaves() const;
    void DeleteSaveFile(const unsigned int& index) const;
    std::string GetSaveName(const unsigned int& index) const;
    unsigned int GetSavesCount() const;
    std::string GetPathOfSave(const unsigned int & index) const;
};

#endif // SAVESREADER_H
