#include "competitor.h"

Competitor::Competitor()
{
    this->name = "";
    this->groupId = 0;
}
Competitor::Competitor(const Competitor& other)
{
    this->groupId = other.groupId;
    this->name = other.name;
}
Competitor::Competitor(const std::string& name, const unsigned int& groupId)
{
    this->name = name;
    this->groupId = groupId;
}

std::string Competitor::GetName() const
{
    return this->name;
}
unsigned int Competitor::GetGroupId() const
{
    return this->groupId;
}

void Competitor::SetName(const std::string& name)
{
    this->name = name;
}
void Competitor::SetGroupId(const unsigned int& groupId)
{
    this->groupId = groupId;
}
