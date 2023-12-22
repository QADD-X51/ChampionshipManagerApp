#include "event.h"

Event::Event() {}

Event::Event(const std::string& name)
{
    this->name = name;
}

Event::Event(const std::string& name, const std::vector<unsigned int>& driverInPos, const std::vector<int>& bonusPoin, const std::vector<std::string>& notes, const std::vector<bool>& isDisqualified)
{
    this->name = name;
    this->notes = notes;
    this->isDisqualified = isDisqualified;
    this->driverBonusPoints = bonusPoin;

    for(unsigned int index=0;index<driverInPos.size();++index)
    {
        this->driversOrderedByPossition[index] = driverInPos.at(index);
    }

    //this->MoveDisqualifiedToBottom();
}
Event::Event(const Event& other)
{
    this->name = other.name;
    this->driversOrderedByPossition = other.driversOrderedByPossition;
    this->driverBonusPoints = other.driverBonusPoints;
    this->notes = other.notes;
    this->isDisqualified = other.isDisqualified;
}

std::string Event::GetName() const
{
    return this->name;
}
void Event::SetName(const std::string newName)
{
    this->name = newName;
}

unsigned int Event::GetDriverIdInPosition(const unsigned int& position) const
{
    return this->driversOrderedByPossition.at(position);
}
int Event::GetBonusPointsOfDriver(const unsigned int& driverId) const
{
    return this->driverBonusPoints.at(driverId);
}
std::string Event::GetNoteOfDriver(const unsigned int& driverId) const
{
    return this->notes.at(driverId);
}
bool Event::DisqualifiedOfDriver(const unsigned int& driverId) const
{
    return this->isDisqualified.at(driverId);
}

void Event::UpdateDriver(const unsigned int& driverId, const unsigned int& newPosition, const int& bonusPoints, const std::string& note, const bool& disqualified)
{
    this->driverBonusPoints.at(driverId) = bonusPoints;
    this->notes.at(driverId) = note;
    this->isDisqualified.at(driverId) = disqualified;

    if(this->driversOrderedByPossition[newPosition] == driverId)
        return;

    unsigned int driverCurrentPosition = this->GetPositionOfDriver(driverId);

    if(driverCurrentPosition > newPosition)
    {
        this->RaiseDriverPosition(driverCurrentPosition, newPosition);
        return;
    }

    this->DropDriverPosition(driverCurrentPosition, newPosition);
}
void Event::LoadDrive(const unsigned int& position, const unsigned int& bonusPoints, const std::string& note, const bool& disqualified)
{
    unsigned int newDriverId = this->driversOrderedByPossition.size();
    this->driversOrderedByPossition[position] = newDriverId;
    this->driverBonusPoints.push_back(bonusPoints);
    this->notes.push_back(note);
    this->isDisqualified.push_back(disqualified);
}
void Event::AddNewEmptyDriver()
{
    unsigned int newDriverId = this->driversOrderedByPossition.size();
    this->driversOrderedByPossition[newDriverId] = newDriverId;
    this->driverBonusPoints.push_back(0);
    this->notes.push_back("Did not take part");
    this->isDisqualified.push_back(true);
}
void Event::RemoveDriver(const unsigned int& driverId)
{
    this->driverBonusPoints.erase(this->driverBonusPoints.begin() + driverId);
    this->notes.erase(this->notes.begin() + driverId);
    this->isDisqualified.erase(this->isDisqualified.begin() + driverId);

    bool removedCompetitor = false;

    for(int index=0;index<this->driversOrderedByPossition.size();++index)
    {
        if(driverId == this->driversOrderedByPossition.at(index))
        {
            removedCompetitor = true;
            continue;
        }
        if(driverId < this->driversOrderedByPossition.at(index))
        {
            --this->driversOrderedByPossition.at(index);
        }
        if(removedCompetitor)
        {
            this->driversOrderedByPossition.at(index - 1) = this->driversOrderedByPossition.at(index);
        }
    }

    this->driversOrderedByPossition.erase(this->driversOrderedByPossition.size() - 1);
}
unsigned int Event::GetPositionOfDriver(const unsigned int& driverId) const
{
    for(const auto& position : this->driversOrderedByPossition)
    {
        if(position.second == driverId)
        {
            return position.first;
        }
    }
}

void Event::RaiseDriverPosition(const unsigned int& initialDriverPosition, const unsigned int& newPosition)
{
    unsigned int temp;
    unsigned int driverId = this->driversOrderedByPossition[initialDriverPosition];
    for(unsigned int index = initialDriverPosition; index > newPosition; --index)
    {
        temp = this->driversOrderedByPossition[index];
        this->driversOrderedByPossition[index] = this->driversOrderedByPossition[index - 1];
        this->driversOrderedByPossition[index - 1] = temp;
    }
}
void Event::DropDriverPosition(const unsigned int& initialDriverPosition, const unsigned int& newPosition)
{
    unsigned int temp;
    unsigned int driverId = this->driversOrderedByPossition[initialDriverPosition];
    for(unsigned int index = initialDriverPosition; index < newPosition; ++index)
    {
        temp = this->driversOrderedByPossition[index];
        this->driversOrderedByPossition[index] = this->driversOrderedByPossition[index + 1];
        this->driversOrderedByPossition[index + 1] = temp;
    }
}

std::unordered_map<unsigned int, unsigned int> Event::GetPositionByDriverIdMap() const
{
    std::unordered_map<unsigned int, unsigned int> positionByDriverId;

    for(const auto& driver : this->driversOrderedByPossition)
    {
        positionByDriverId[driver.second] = driver.first;
    }

    return positionByDriverId;
}

std::vector<unsigned int> Event::PointsOrder(const std::vector<Competitor>& competitors, const unsigned int& groupCount) const
{
    std::vector<std::vector<unsigned int>> newPositions;

    for(unsigned int index = 0;index<groupCount;++index)
    {
        newPositions.push_back(std::vector<unsigned int>());
    }

    for(unsigned int position = 0; position < this->driversOrderedByPossition.size(); ++position)
    {
        newPositions.at(competitors.at(position).GetGroupId()).push_back(this->driversOrderedByPossition.at(position));
    }

    std::vector<unsigned int> toReturn;

    for(unsigned int group = 0; group<groupCount;++group)
    {
        for(unsigned int index = 0;index < newPositions.at(group).size();++index)
        {
            toReturn.push_back(newPositions.at(group).at(index));
        }
    }

    return toReturn;
}

unsigned int Event::PositionOfCompetitorByGroup(const std::vector<Competitor>& competitors, const unsigned int& competitorTargetId, const unsigned int& groupTargetId) const
{
    unsigned int positionOfTarget = 0;

    for(unsigned int position=0;position<competitors.size();++position)
    {
        if(this->driversOrderedByPossition.at(position) == competitorTargetId)
            return positionOfTarget;

        if(competitors.at(this->driversOrderedByPossition.at(position)).GetGroupId() == groupTargetId)
            ++positionOfTarget;
    }

    return 0;
}

void Event::MoveDisqualifiedToBottom()
{
    //Fix THIS
    unsigned int limit = this->isDisqualified.size();
    for(unsigned int index=0;index< limit;++index)
    {
        if(this->isDisqualified.at(index) == false)
        {
            continue;
        }
        --index;
        --limit;
        this->DropDriverPosition(index, this->isDisqualified.size() - 1);
    }
}

std::vector<unsigned int> Event::DriversOrderToVector(const unsigned int& groupId, const std::vector<Competitor>& competitors) const
{
    std::vector<unsigned int> toReturn;

    for(unsigned int index=0;index<this->driverBonusPoints.size();++index)
    {
        toReturn.push_back(0);
    }

    for(const auto& item : this->driversOrderedByPossition)
    {
        toReturn.at(item.first) = item.second;
    }

    if(groupId == 0 || competitors.size() == 0)
    {
        return toReturn;
    }

    std::vector<unsigned int> aux;

    for(int index=0;index<competitors.size();++index)
    {
        if(competitors.at(index).GetGroupId() == groupId - 1)
        {
            aux.push_back(toReturn.at(index));
        }
    }

    for(unsigned int count = 0;count<aux.size(); ++count)
    {
        int mini = INT32_MAX;
        for(const auto& position:aux)
        {
            if(position < mini && position >= count)
            {
                mini = position;
            }
            if(mini == count) break;
        }
        if(mini == count) continue;

        for(auto& position: aux)
        {
            if(position == mini)
            {
                position = count;
                break;
            }
        }
    }

    return aux;
}
