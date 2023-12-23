#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include <string>
#include <vector>
#include <tuple>

#include "competitor.h"
#include "event.h"

class Championship
{
    std::string filePath;
    std::string name;
    std::string description;
    std::vector<std::vector<int>> positionsPoints; //positionsPoints[group][position]
    std::vector<Competitor> competitors;
    std::vector<std::string> groupNames;
    std::vector<Event> events;

    unsigned int groupsCount;
    unsigned int competitorsCount;
    unsigned int eventsCount;
    std::vector<unsigned int> groupsCompetitorsCount;
public:
    Championship(const std::string& filePath);
    Championship(const Championship& other);

    std::string GetFilePath() const;
    std::string GetName() const;
    std::vector<std::vector<int>> GetPoints() const;
    std::vector<Competitor> GetAllCompetitors() const;
    std::vector<std::string> GetGroupNames() const;
    std::vector<Event> GetEvents() const;
    std::vector<std::string> GetAllEventNames() const;

    unsigned int GetGroupCount() const;
    unsigned int GetCompetitorsCount() const;
    unsigned int GetEventsCount() const;
    unsigned int GetCompetitorsCountInGroup(const unsigned int& index) const;
    int GetPointsInPosition(const unsigned int& position, const unsigned int& groupId) const;
    std::string GetDescription() const;
    Event GetEvent(const unsigned int& eventId) const;

    Competitor GetCompetitor(const unsigned int& index) const;
    std::string GetGroupName(const unsigned int& index) const;

    void AddCompetitor(const std::string& name, const unsigned int& groupId);
    void RemoveCompetitor(const unsigned int& driveId);
    void UpdateCompetitor(const unsigned int& driverId, const std::string& name, const unsigned int& groupId);
    void SwapCompetitors(const unsigned int& driverId1, const unsigned int& driverId2);
    void ReorderCompetitors();

    void AddGroup(const std::string& name);
    void RemoveGroup(const unsigned int& groupId);
    void UpdateGroup(const unsigned int& groupId, const std::string& name);
    void SwapGroups(const unsigned int& groupId1, const unsigned int& groupId2);

    void AddEvent(const Event& event);
    void RemoveEvent(const unsigned int& eventId);
    void ChangeEventName(const unsigned int& eventId, const std::string& newName);
    void UpdateEventCompetitor(const unsigned int& eventId, const unsigned int& competitorId, const unsigned int& newPosition, const int& bonusPoints, const std::string& note, const bool& isDisq);
    void SwapEvents(const unsigned int& eventId1, const unsigned int& eventId2);

    void SetName(const std::string& name);
    void SetPoints(const int& points, const unsigned int& position, const unsigned int& groupId);
    void SetDescription(const std::string& str);

    void SaveToFile();

    std::vector<std::tuple<std::string, std::string, int, std::string>> CalculateOverviewPoints(const unsigned int& groupId = 0) const;
    std::vector<std::tuple<std::string,std::string,int,int,int,std::string, std::string, std::string>> CalculateEventPoints(const unsigned int& eventId, const unsigned int& groupId = 0, const bool& reorder = false) const;

private:

    void ReadFromFile();

    void SaveGroup(std::ofstream& out, const unsigned int& index);
    void SavePoints(std::ofstream& out, const unsigned int& index);
    void SaveCompetitors(std::ofstream& out, const unsigned int& index);
    void SaveEvents(std::ofstream& out, const unsigned int& index);

    void LoadGroup(std::ifstream& in);
    void LoadPoints(std::ifstream& in, const unsigned int& group);
    void LoadCompetitors(std::ifstream& in);
    void LoadEvents(std::ifstream& in);
};

#endif // CHAMPIONSHIP_H
