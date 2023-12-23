#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <unordered_map>
#include <vector>

#include "competitor.h"

class Event
{

    std::string name;
    std::unordered_map<unsigned int, unsigned int> driversOrderedByPossition; //driverInPosition[position] = driverId
    std::vector<int> driverBonusPoints;  //driverBonusPoints[driverId] = points;
    std::vector<std::string> notes;
    std::vector<bool> isDisqualified;

public:

    Event();
    Event(const std::string& name, const std::vector<unsigned int>& driverInPos, const std::vector<int>& bonusPoin, const std::vector<std::string>& notes, const std::vector<bool>& isDisqualified);
    Event(const Event& other);
    Event(const std::string& name);

    std::string GetName() const;
    void SetName(const std::string newName);

    unsigned int GetDriverIdInPosition(const unsigned int& position) const;
    int GetBonusPointsOfDriver(const unsigned int& driverId) const;
    std::string GetNoteOfDriver(const unsigned int& driverId) const;
    bool DisqualifiedOfDriver(const unsigned int& driverId) const;

    unsigned int GetPositionOfDriver(const unsigned int& driverId) const;
    std::vector<unsigned int> DriversOrderToVector(const unsigned int& groupId = 0, const std::vector<Competitor>& competitors = std::vector<Competitor>()) const;

    void UpdateDriver(const unsigned int& driverId, const unsigned int& newPosition, const int& bonusPoints, const std::string& note, const bool& disqualified = false);
    void LoadDrive(const unsigned int& position, const unsigned int& bonusPoints, const std::string& note, const bool& disqualified);

    std::unordered_map<unsigned int, unsigned int> GetPositionByDriverIdMap() const;
    std::vector<unsigned int> PointsOrder(const std::vector<Competitor>& competitors, const unsigned int& groupdCount) const;
    unsigned int PositionOfCompetitorByGroup(const std::vector<Competitor>& competitors, const unsigned int& competitorTargetId, const unsigned int& groupTargetId) const;

    void AddNewEmptyDriver();
    void RemoveDriver(const unsigned int& driverId);
    void SwapDrivers(const unsigned int& driverId1, const unsigned int& driverId2);

private:

    void RaiseDriverPosition(const unsigned int& driverId, const unsigned int& newPosition);
    void DropDriverPosition(const unsigned int& driverId, const unsigned int& newPosition);

    void MoveDisqualifiedToBottom();
};

#endif // EVENT_H
