#ifndef COMPETITOR_H
#define COMPETITOR_H

#include <string>

class Competitor
{
    std::string name;
    unsigned int groupId;

public:
    Competitor();
    Competitor(const std::string& name, const unsigned int& groupId);
    Competitor(const Competitor& other);

    std::string GetName() const;
    unsigned int GetGroupId() const;

    void SetName(const std::string& name);
    void SetGroupId(const unsigned int& groupId);
};

#endif // COMPETITOR_H
