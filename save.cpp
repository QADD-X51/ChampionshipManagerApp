#include "save.h"

Save::Save(const std::string& path, const std::string& championship_name, const unsigned int& nr_of_events, const unsigned int& nr_of_competitors)
{
    this->path = path;
    this->championship_name = championship_name;
    this->number_of_events = new unsigned int;
    this->number_of_competitors = new unsigned int;

    *(this->number_of_events) = nr_of_events;
    *(this->number_of_competitors) = nr_of_competitors;
}

Save::Save(const Save &other)
{
    this->path = other.GetPath();
    this->championship_name = other.GetName();
    this->number_of_events = new unsigned int;
    this->number_of_competitors = new unsigned int;

    *(this->number_of_events) = other.GetEventsCount();
    *(this->number_of_competitors) = other.GetCompetitorsCount();
}

std::string Save::GetPath() const
{
    return this->path;
}

std::string Save::GetName() const
{
    return this->championship_name;
}

unsigned int Save::GetEventsCount() const
{
    return *this->number_of_events;
}

unsigned int Save::GetCompetitorsCount() const
{
    return *this->number_of_competitors;
}

Save::~Save()
{
    delete this->number_of_competitors;
    delete this->number_of_events;
}
