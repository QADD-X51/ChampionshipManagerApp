#ifndef SAVE_H
#define SAVE_H

#include <string>

class Save
{
private:

    std::string path;
    std::string championship_name;
    unsigned int* number_of_events;
    unsigned int* number_of_competitors;

public:
    Save(const std::string& path, const std::string& championship_name, const unsigned int& nr_of_events, const unsigned int& nr_of_competitors);
    Save(const Save &other);

    std::string GetPath() const;
    std::string GetName() const;
    unsigned int GetEventsCount() const;
    unsigned int GetCompetitorsCount() const;

    ~Save();

};

#endif // SAVE_H
