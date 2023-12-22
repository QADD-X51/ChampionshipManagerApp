#include "addeventwindow.h"
#include "ui_addeventwindow.h"

#include "utils.h"
#include <iostream>
#include <QMessageBox>

AddEventWindow::AddEventWindow(Championship* championshipRef, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddEventWindow)
{
    ui->setupUi(this);

    this->championshipRef = championshipRef;

    SetUpScrollArea();
}

AddEventWindow::~AddEventWindow()
{
    delete ui;

    for (auto& item : countLabels)
    {
        delete item;
    }
    for (auto& item : competitorNameLabels)
    {
        delete item;
    }
    for (auto& item : groupNameLabels)
    {
        delete item;
    }
    for (auto& item : positionCombos)
    {
        delete item;
    }
    for (auto& item : minPositionCombos)
    {
        delete item;
    }
    for (auto& item : maxPositionCombos)
    {
        delete item;
    }
    for (auto& item : bonusPointsSpins)
    {
        delete item;
    }
    for (auto& item : noteBoxs)
    {
        delete item;
    }
    for (auto& item : disqualifiedChecks)
    {
        delete item;
    }
}

void AddEventWindow::SetUpScrollArea()
{
    QStringList positionList;
    for(unsigned int index=0;index < this->championshipRef->GetCompetitorsCount();++index)
    {
        positionList.append(QString::fromStdString(utils::IntToPositionString(index+1)));
    }

    for(unsigned int index=0;index < this->championshipRef->GetCompetitorsCount();++index)
    {
        QLabel* countLabel = new QLabel;
        QLabel* competitorNameLabel = new QLabel;
        QLabel* groupNameLabel = new QLabel;
        QComboBox* selectPositionBox = new QComboBox;
        QComboBox* minRandomPositionBox = new QComboBox;
        QComboBox* maxRandomPositionBox = new QComboBox;
        QSpinBox* bonusPointsSpin = new QSpinBox;
        QTextEdit* noteText = new QTextEdit;
        QCheckBox* isDisqualified = new QCheckBox;

        countLabel->setText(QString::fromStdString(std::to_string(index + 1) + "."));
        competitorNameLabel->setText(QString::fromStdString(this->championshipRef->GetCompetitor(index).GetName()));
        groupNameLabel->setText(QString::fromStdString(this->championshipRef->GetGroupName(this->championshipRef->GetCompetitor(index).GetGroupId())));

        selectPositionBox->addItem("Random");
        selectPositionBox->addItems(positionList);
        minRandomPositionBox->addItems(positionList);
        maxRandomPositionBox->addItems(positionList);
        maxRandomPositionBox->setCurrentIndex(positionList.size() - 1);

        bonusPointsSpin->setValue(0);
        bonusPointsSpin->setMinimum(-1000000);
        bonusPointsSpin->setMaximum(1000000);


        ui->scrollGrid->addWidget(countLabel, index + 1, 0);
        ui->scrollGrid->addWidget(competitorNameLabel, index + 1, 1);
        ui->scrollGrid->addWidget(groupNameLabel, index + 1, 2);
        ui->scrollGrid->addWidget(selectPositionBox, index + 1, 3);
        ui->scrollGrid->addWidget(bonusPointsSpin, index + 1, 4);
        ui->scrollGrid->addWidget(noteText, index + 1, 6);
        ui->scrollGrid->addWidget(isDisqualified, index + 1, 5);
        ui->scrollGrid->addWidget(minRandomPositionBox, index + 1, 7);
        ui->scrollGrid->addWidget(maxRandomPositionBox, index + 1, 8);

        countLabels.append(countLabel);
        competitorNameLabels.append(competitorNameLabel);
        groupNameLabels.append(groupNameLabel);
        positionCombos.append(selectPositionBox);
        minPositionCombos.append(minRandomPositionBox);
        maxPositionCombos.append(maxRandomPositionBox);
        bonusPointsSpins.append(bonusPointsSpin);
        noteBoxs.append(noteText);
        disqualifiedChecks.append(isDisqualified);
    }


}

void AddEventWindow::on_addEventeButton_clicked()
{
    QMessageBox box;
    box.setModal(true);

    if(ui->eventNameBox->text().toStdString().empty() || ui->eventNameBox->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {
        box.warning(this, "Wrong Input", QString::fromStdString("The imputs are wrong. Make sure the following conditins are meet:" \
                                                                "\n1. The name of the event can not be blank." \
                                                                "\n2. All competitors must be in unique positions." \
                                                                "\n3. In the case of random positions, their minimum and maximum position must allow them to occupy at least a free slot."));

        return;
    }

    std::vector<unsigned int> positionsVector = this->MakePositionsVector();
    if(positionsVector.empty())
    {
        box.warning(this, "Wrong Input", QString::fromStdString("The imputs are wrong. Make sure the following conditins are meet:" \
                                                                "\n1. The name of the event can not be blank." \
                                                                "\n2. All competitors must be in unique positions." \
                                                                "\n3. In the case of random positions, their minimum and maximum position must allow them to occupy at least a free slot."));
        return;
    }

    this->BuildEvent(positionsVector);

    this->accept();
}

std::vector<unsigned int> AddEventWindow::MakePositionsVector() const
{
    std::vector<unsigned int> toReturn;

    for(unsigned int index=0;index<this->championshipRef->GetCompetitorsCount();++index)
    {
        toReturn.push_back(0);
    }

    for(unsigned int index=0;index<this->positionCombos.size();++index)
    {
        if(this->disqualifiedChecks.at(index)->isChecked())
        {
            continue;
        }
        if(this->positionCombos.at(index)->currentIndex() == 0)
        {
            continue;
        }
        if(toReturn.at(this->positionCombos.at(index)->currentIndex() - 1) != 0)
        {
            return std::vector<unsigned int>();
        }
        toReturn.at(this->positionCombos.at(index)->currentIndex() - 1) = index + 1;
    }

    std::vector<unsigned int> remainingRandom;

    for(unsigned int index=0;index<this->positionCombos.size();++index)
    {
        if(this->disqualifiedChecks.at(index)->isChecked())
        {
            continue;
        }
        if(this->positionCombos.at(index)->currentIndex() != 0)
        {
            continue;
        }
        std::vector<unsigned int> availablePosition = utils::MakeArray(this->minPositionCombos.at(index)->currentIndex(), this->maxPositionCombos.at(index)->currentIndex() + 1);

        for(int jndex=0;jndex<availablePosition.size();++jndex)
        {
            if(toReturn.at(availablePosition.at(jndex)) != 0)
            {
                availablePosition.erase(availablePosition.begin() + jndex);
                --jndex;
            }
        }

        if(!availablePosition.size())
        {
            remainingRandom.push_back(index);
            continue;
        }

        toReturn.at(utils::PickRandomElementFromArray(availablePosition)) = index + 1;
    }

    if(!remainingRandom.empty())
    {
        std::vector<unsigned int> availablePosition;

        for(unsigned int index=0;index < toReturn.size();++index)
        {
            if(!toReturn.at(index))
            {
                availablePosition.push_back(index);
            }
        }

        for(const auto& rando:remainingRandom)
        {
            unsigned int index = utils::GenerateRandomUnsignedInt(0, availablePosition.size());
            toReturn.at(availablePosition.at(index)) = rando + 1;
            availablePosition.erase(availablePosition.begin() + index);
        }
    }

    unsigned int limit = toReturn.size();
    for(int index=0;index<limit - 1;++index)
    {
        if(toReturn.at(index) == 0)
        {
            for(int jndex=0;jndex<limit;++jndex)
            {
                unsigned int aux = toReturn.at(index);
                toReturn.at(index) = toReturn.at(jndex);
                toReturn.at(jndex) = aux;
            }
            --limit;
            --index;
        }
    }

    limit = 0;
    for(unsigned int index=0;index<positionCombos.size();++index)
    {
        if(!this->disqualifiedChecks.at(index)->isChecked())
        {
            continue;
        }

        toReturn.at(toReturn.size() - 1 - limit) = index + 1;
        ++limit;
    }

    for(unsigned int index=0;index<toReturn.size();++index)
    {
        --toReturn.at(index);
    }

    return toReturn;
}

void AddEventWindow::BuildEvent(const std::vector<unsigned int>& positionArray) const
{
    std::vector<std::string> notes;
    std::vector<int> bonusPoints;
    std::vector<bool> isDisq;

    for(const auto& item : this->bonusPointsSpins)
    {
        bonusPoints.push_back(item->value());
    }

    for(const auto& item : this->noteBoxs)
    {
        notes.push_back(item->toPlainText().toStdString());
    }

    for(const auto& item : this->disqualifiedChecks)
    {
        isDisq.push_back(item->isChecked());
    }

    Event event(ui->eventNameBox->text().toStdString(), positionArray, bonusPoints, notes, isDisq);
    this->championshipRef->AddEvent(event);
}
