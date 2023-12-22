#include "championship.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"

Championship::Championship(const std::string& filePath)
{
    this->filePath = filePath;

    ReadFromFile();
}

Championship::Championship(const Championship& other)
{
    this->filePath = other.filePath;
    this->name = other.name;
    this->positionsPoints = other.positionsPoints;
    this->groupsCount = other.groupsCount;
    this->groupNames = other.groupNames;
    this->groupsCompetitorsCount = other.groupsCompetitorsCount;
    this->eventsCount = other.eventsCount;
    this->events = other.events;
    this->competitorsCount = other.competitorsCount;
    this->competitors = other.competitors;
    this->description = other.description;
}

std::string Championship::GetFilePath() const
{
    return this->filePath;
}
std::string Championship::GetName() const
{
    return this->name;
}
std::vector<std::vector<int>> Championship::GetPoints() const
{
    return this->positionsPoints;
}
std::vector<Competitor> Championship::GetAllCompetitors() const
{
    return this->competitors;
}
std::vector<std::string> Championship::GetGroupNames() const
{
    return this->groupNames;
}
std::vector<Event> Championship::GetEvents() const
{
    return this->events;
}
std::string Championship::GetDescription() const
{
    return this->description;
}

void Championship::ReadFromFile()
{
    std::ifstream in(this->filePath);

    std::getline(in, this->name);
    in >> this->eventsCount >> this->competitorsCount >> this->groupsCount;

    std::string dummy;
    std::getline(in, dummy);

    this->description = "";

    while(true)
    {
        std::getline(in, dummy);
        if(dummy.size() == 1)
            if(dummy[0] == utils::endOfNoteChar)
                break;

        this->description += dummy + "\n";
    }

    for(unsigned int index=0;index<this->groupsCount;++index)
    {
        this->LoadGroup(in);
    }

    for(unsigned int groupIndex = 0; groupIndex<groupsCount;++groupIndex)
    {
        for (unsigned int index=0;index<this->groupsCompetitorsCount.at(groupIndex);++index)
        {
            this->LoadPoints(in, groupIndex);
        }

    }

    for(unsigned int index=0;index< this->competitorsCount;++index)
    {
        this->LoadCompetitors(in);
    }

    for(unsigned int index=0;index< this->eventsCount;++index)
    {
        this->LoadEvents(in);
    }
}

void Championship::SaveToFile()
{
    std::ofstream out(this->filePath);

    out << this->name << std::endl;
    out << this->eventsCount << ' ' << this->competitorsCount << ' ' << this->groupsCount << std::endl;

    out << this->description << std::endl;
    out << utils::endOfNoteChar << std::endl;

    for(unsigned int index=0;index<this->groupsCount;++index)
    {
        this->SaveGroup(out, index);
    }

    for(unsigned int index=0;index< this->groupsCount;++index)
    {
        this->SavePoints(out, index);
    }

    for(unsigned int index=0;index< this->competitorsCount;++index)
    {
        this->SaveCompetitors(out, index);
    }

    for(unsigned int index=0;index< this->eventsCount;++index)
    {
        this->SaveEvents(out, index);
    }
}

void Championship::SaveGroup(std::ofstream& out, const unsigned int& index)
{
    out << this->groupNames.at(index) << std::endl;
    out << this->groupsCompetitorsCount.at(index) << std::endl;
}
void Championship::SavePoints(std::ofstream& out, const unsigned int& index)
{
    for(const auto& points : this->positionsPoints.at(index))
        out << points << ' ';
    out<<std::endl;
}
void Championship::SaveCompetitors(std::ofstream& out, const unsigned int& index)
{
    out << this->competitors.at(index).GetGroupId() << std::endl;
    out << this->competitors.at(index).GetName() << std::endl;
}
void Championship::SaveEvents(std::ofstream& out, const unsigned int& index)
{

    out << this->events.at(index).GetName() << std::endl;

    for(unsigned int jndex=0;jndex<this->competitorsCount;++jndex)
    {
        out << this->events.at(index).GetPositionOfDriver(jndex) << ' ' << this->events.at(index).GetBonusPointsOfDriver(jndex) << ' ';
        out << this->events.at(index).DisqualifiedOfDriver(jndex) << std::endl;
        out << this->events.at(index).GetNoteOfDriver(jndex) << std::endl;
        out << utils::endOfNoteChar << std::endl;
    }
}

void Championship::LoadGroup(std::ifstream& in)
{
    std::string name;
    unsigned int count;

    std::getline(in, name);
    in >> count;
    this->groupNames.push_back(name);
    this->groupsCompetitorsCount.push_back(count);
    this->positionsPoints.push_back(std::vector<int>());
    std::getline(in,name);
}
void Championship::LoadPoints(std::ifstream& in, const unsigned int& group)
{
    unsigned int temp;
    in >> temp;
    this->positionsPoints.at(group).push_back(temp);
}
void Championship::LoadCompetitors(std::ifstream& in)
{
    std::string name;
    unsigned int groupId;


    in >> groupId;
    std::getline(in, name);
    std::getline(in, name);

    this->competitors.emplace_back(Competitor(name, groupId));
}
void Championship::LoadEvents(std::ifstream& in)  // Event Name, for each competitor: position bonusPts isDisq notes
{
    std::string note;
    int bonusPoints;
    unsigned int position;
    bool disq;

    std::getline(in, note);

    Event event(note);

    for(unsigned int index=0;index<this->competitorsCount;++index)
    {
        in>>position >> bonusPoints >> disq;
        std::getline(in, note);

        std::string noteToAdd = "";
        while(true)
        {
            std::getline(in, note);
            if(note.size() == 1)
                if(note[0] == utils::endOfNoteChar)
                    break;

            noteToAdd += note + "\n";
        }
        event.LoadDrive(position, bonusPoints, noteToAdd, disq);
    }

    this->events.emplace_back(event);
}

unsigned int Championship::GetGroupCount() const
{
    return this->groupsCount;
}
unsigned int Championship::GetCompetitorsCount() const
{
    return this->competitorsCount;
}
unsigned int Championship::GetEventsCount() const
{
    return this->eventsCount;
}
unsigned int Championship::GetCompetitorsCountInGroup(const unsigned int& index) const
{
    return this->groupsCompetitorsCount.at(index);
}
int Championship::GetPointsInPosition(const unsigned int& position, const unsigned int& groupId) const
{
    return this->positionsPoints.at(groupId).at(position);
}
Event Championship::GetEvent(const unsigned int& eventId) const
{
    return this->events.at(eventId);
}

std::vector<std::string> Championship::GetAllEventNames() const
{
    std::vector<std::string> to_return;
    for(const auto& event : this->events)
    {
        to_return.push_back(event.GetName());
    }
    return to_return;
}

std::vector<std::tuple<std::string, std::string, int, std::string>> Championship::CalculateOverviewPoints(const unsigned int& groupId) const
{
    using Tuple = std::tuple<std::string, std::string, int, std::string>;
    std::vector<Tuple> to_return;

    for(unsigned int index = 0; index<this->competitorsCount;++index)
    {
        if(groupId != 0 && this->competitors.at(index).GetGroupId() != groupId - 1)
        {
            continue;
        }

        std::string competitorName = this->competitors.at(index).GetName();
        std::string groupName = this->groupNames.at(this->competitors.at(index).GetGroupId());

        int points = 0;
        for(const Event& event : this->events)
        {
            if(!event.DisqualifiedOfDriver(index))
            {
                unsigned int positionInGroup = event.PositionOfCompetitorByGroup(this->competitors, index, this->competitors.at(index).GetGroupId());
                points += this->positionsPoints.at(this->competitors.at(index).GetGroupId()).at(positionInGroup) + event.GetBonusPointsOfDriver(index);
            }

        }

        to_return.push_back(std::make_tuple("", competitorName, points, groupName));
    }

    std::sort(to_return.begin(), to_return.end(),
              [](const Tuple& a, const Tuple& b)
              { return std::get<2>(a) > std::get<2>(b);});

    for(unsigned int index=0;index<to_return.size();++index)
    {
        std::get<0>(to_return.at(index)) = utils::IntToPositionString(index + 1);
    }

    return to_return;
}

//position, name, points, bonus p, total p, group, notes, disqualified
std::vector<std::tuple<std::string,std::string,int,int,int,std::string, std::string, std::string>> Championship::CalculateEventPoints(const unsigned int& eventId, const unsigned int& groupId, const bool& reorder) const
{
    using Tuple = std::tuple<std::string,std::string,int,int,int,std::string, std::string, std::string>;
    std::vector<Tuple> to_return;

    for(unsigned int index = 0; index<this->competitorsCount;++index)
    {
        if(groupId != 0 && this->competitors.at(index).GetGroupId() != groupId - 1)
        {
            continue;
        }

        std::string competitorName = this->competitors.at(index).GetName();
        std::string groupName = this->groupNames.at(this->competitors.at(index).GetGroupId());

        bool isDisqualified = this->events.at(eventId).DisqualifiedOfDriver(index);

        int points = 0;
        unsigned int eventCount = 0;
        for(const Event& event : this->events)
        {
            if(eventCount++ > eventId)
            {
                break;
            }
            if(!event.DisqualifiedOfDriver(index))
            {
                unsigned int positionInGroup = event.PositionOfCompetitorByGroup(this->competitors, index, this->competitors.at(index).GetGroupId());
                points += this->positionsPoints.at(this->competitors.at(index).GetGroupId()).at(positionInGroup) + event.GetBonusPointsOfDriver(index);
            }

        }

        int currentPoints = 0;
        if(!isDisqualified)
        {
            currentPoints = this->positionsPoints.at(this->competitors.at(index).GetGroupId()).at(this->events.at(eventId).PositionOfCompetitorByGroup(this->competitors, index, this->competitors.at(index).GetGroupId()));
        }

        int bonusPoints = this->events.at(eventId).GetBonusPointsOfDriver(index);
        std::string note = this->events.at(eventId).GetNoteOfDriver(index);
        std::string disqStr = isDisqualified ? "Yes": "No";

        to_return.push_back(std::make_tuple("", competitorName, currentPoints,bonusPoints,points, groupName, note, disqStr));
    }

    if(reorder)
    {
        std::sort(to_return.begin(), to_return.end(),
                  [](const Tuple& a, const Tuple& b)
                  { return std::get<4>(a) > std::get<4>(b);});
    }
    else
    {
        std::vector<Tuple> temp;

        auto defaultPos = this->events.at(eventId).DriversOrderToVector(groupId, this->competitors);

        for(unsigned int index=0;index<defaultPos.size();++index)
        {
            temp.push_back(to_return.at(defaultPos.at(index)));
        }

        to_return = temp;
    }

    for(unsigned int index=0;index<to_return.size();++index)
    {
        std::get<0>(to_return.at(index)) = utils::IntToPositionString(index + 1);
    }

    return to_return;
}

void Championship::SetName(const std::string& name)
{
    this->name = name;
}
void Championship::SetPoints(const int& points, const unsigned int& position, const unsigned int& groupId)
{
    this->positionsPoints.at(groupId).at(position) = points;
}
void Championship::SetDescription(const std::string& str)
{
    this->description = str;
}

Competitor Championship::GetCompetitor(const unsigned int& index) const
{
    return this->competitors.at(index);
}

std::string Championship::GetGroupName(const unsigned int& index) const
{
    return this->groupNames.at(index);
}

void Championship::AddCompetitor(const std::string& name, const unsigned int& groupId)
{
    Competitor newCompetitor(name, groupId);

    this->competitors.push_back(newCompetitor);
    ++this->competitorsCount;
    ++this->groupsCompetitorsCount.at(groupId);

    this->positionsPoints.at(groupId).push_back(0);

    for(auto& event : this->events)
    {
        event.AddNewEmptyDriver();
    }

}
void Championship::AddGroup(const std::string& name)
{
    this->groupNames.push_back(name);
    this->groupsCompetitorsCount.push_back(0);
    ++this->groupsCount;
}
void Championship::RemoveGroup(const unsigned int& groupId)
{
    std::vector<unsigned int> competitorsToRemove;

    this->groupNames.erase(this->groupNames.begin() + groupId);

    for(unsigned int index = 0;index<this->competitorsCount;++index)
    {
        if(this->competitors.at(index).GetGroupId() == groupId)
        {
            competitorsToRemove.push_back(index);
        }
    }

    for(auto& event : this->events)
    {
        for(const auto& competitor : competitorsToRemove)
        {
            event.RemoveDriver(competitor);
        }
    }

    for(const auto& competitor : competitorsToRemove)
    {
        this->competitors.erase(this->competitors.begin() + competitor);
        --this->competitorsCount;
    }

    for(auto& competitor:this->competitors)
    {
        if(competitor.GetGroupId() > groupId)
            competitor.SetGroupId(competitor.GetGroupId() - 1);
    }

    this->positionsPoints.erase(this->positionsPoints.begin() + groupId);
    this->groupsCompetitorsCount.erase(this->groupsCompetitorsCount.begin() + groupId);
    --this->groupsCount;
}
void Championship::UpdateGroup(const unsigned int& groupId, const std::string& name)
{
    this->groupNames.at(groupId) = name;
}

void Championship::RemoveCompetitor(const unsigned int& driverId)
{
    --this->competitorsCount;
    --this->groupsCompetitorsCount.at(this->competitors.at(driverId).GetGroupId());

    this->positionsPoints.at(this->competitors.at(driverId).GetGroupId()).erase(this->positionsPoints.at(this->competitors.at(driverId).GetGroupId()).begin() + this->competitors.at(driverId).GetGroupId());

    this->competitors.erase(this->competitors.begin() + driverId);

    for(auto& event : this->events)
    {
        event.RemoveDriver(driverId);
    }

}

void Championship::UpdateCompetitor(const unsigned int& driverId, const std::string& name, const unsigned int& groupId)
{
    this->competitors.at(driverId).SetName(name);
    this->competitors.at(driverId).SetGroupId(groupId);
}

void Championship::AddEvent(const Event& event)
{
    this->events.push_back(event);
    ++this->eventsCount;
}

void Championship::RemoveEvent(const unsigned int& eventId)
{
    this->events.erase(this->events.begin() + eventId);
    --this->eventsCount;
}

void Championship::ChangeEventName(const unsigned int& eventId, const std::string& newName)
{
    this->events.at(eventId).SetName(newName);
}

void Championship::UpdateEventCompetitor(const unsigned int& eventId, const unsigned int& competitorId, const unsigned int& newPosition, const int& bonusPoints, const std::string& note, const bool& isDisq)
{
    this->events.at(eventId).UpdateDriver(competitorId, newPosition, bonusPoints, note, isDisq);
}
