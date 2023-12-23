#include "managechampionshipwindow.h"
#include "ui_managechampionshipwindow.h"

#include <QMessageBox>
#include "utils.h"

ManageChampionshipWindow::ManageChampionshipWindow(Championship* refChampionship,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageChampionshipWindow)
{
    ui->setupUi(this);

    this->referenceChampionship = refChampionship;
    this->championship = new Championship(*refChampionship);

    randomNames = new RandomNames();

    savedChanges = true;

    this->SetUpAllTabs();
}

void ManageChampionshipWindow::SetUpAllTabs()
{
    ui->currentChampionshipNameLabel->setText(QString::fromStdString(this->championship->GetName()));
    ui->championshipNameBox->setText(QString::fromStdString(this->championship->GetName()));
    ui->descriptionBox->setPlainText(QString::fromStdString(this->championship->GetDescription()));
    this->SetUpCompetitorsTab();
    this->SetUpPointsTab();
    this->SetUpGroupsTab();
    this->SetUpEventsTab();
}

ManageChampionshipWindow::~ManageChampionshipWindow()
{
    delete ui;
    delete randomNames;
    delete championship;
}

void ManageChampionshipWindow::on_changeChampionshipNameButton_clicked()
{

    if(ui->championshipNameBox->text().toStdString().empty() || ui->championshipNameBox->text().toStdString().find_first_of(".*:/\\\"\'<>,|?") != std::string::npos || ui->championshipNameBox->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {
        QMessageBox box;
        box.warning(this,"Wrong Name","The Name can't be empty or contain the following symbols:\n.*:/\\\"\'<>,|?");
        box.show();
        return;
    }

    this->championship->SetName(ui->championshipNameBox->text().toStdString());
    this->championship->SetDescription(ui->descriptionBox->toPlainText().toStdString());

    ui->currentChampionshipNameLabel->setText(QString::fromStdString(this->championship->GetName()));
    savedChanges = false;
}


void ManageChampionshipWindow::on_saveAllButton_clicked()
{
    *this->referenceChampionship = *this->championship;
    savedChanges = true;

    QMessageBox box;
    box.setModal(true);
    box.information(this, "Saved Championship", "The championship was saved.");
}


void ManageChampionshipWindow::on_selectedDriverBox_currentIndexChanged(int index)
{
    if(index)
    {
        ui->competitorNameBox->setText(QString::fromStdString(this->championship->GetCompetitor(index - 1).GetName()));
        ui->competitorGroupCombo->setCurrentIndex(this->championship->GetCompetitor(index - 1).GetGroupId());
        ui->deleteCompetitorButton->setEnabled(true);
        return;
    }

    ui->competitorNameBox->setText(QString(""));
    ui->competitorGroupCombo->setCurrentIndex(0);
    ui->deleteCompetitorButton->setEnabled(false);
}

void ManageChampionshipWindow::SetUpCompetitorsTab()
{
    ui->selectedCompetitorCombo->blockSignals(true);
    ui->selectedCompetitorCombo->clear();
    ui->selectedCompetitorCombo->blockSignals(false);

    ui->competitorGroupCombo->blockSignals(true);
    ui->competitorGroupCombo->clear();
    ui->competitorGroupCombo->blockSignals(false);

    ui->swapCompetitorBox->blockSignals(true);
    ui->swapCompetitorBox->clear();
    ui->swapCompetitorBox->blockSignals(false);

    ui->swapCompetitorBox->setEnabled(false);
    ui->swapCompetitorsButton->setEnabled(false);

    ui->reorderCompetitorsButton->setEnabled(false);

    if(this->championship->GetCompetitorsCount() > 1)
    {
        ui->reorderCompetitorsButton->setEnabled(true);
    }

    ui->selectedCompetitorCombo->addItem("New Driver");

    unsigned int count = 0;
    for(const auto& competitor : this->championship->GetAllCompetitors())
    {
        ui->selectedCompetitorCombo->addItem(QString::fromStdString(std::to_string(++count) + ". " + competitor.GetName()));
        ui->swapCompetitorBox->addItem(QString::fromStdString(std::to_string(count) + ". " + competitor.GetName()));
    }

    count = 1;
    for(const auto& group : this->championship->GetGroupNames())
    {
        ui->competitorGroupCombo->addItem(QString::fromStdString(std::to_string(count++) + ". " + group));
    }

}

void ManageChampionshipWindow::on_saveCompetitorButton_clicked()
{
    if(ui->competitorNameBox->text().toStdString().find_first_not_of(' ') == std::string::npos || ui->competitorNameBox->text().toStdString() == "")
    {
        QMessageBox box;
        box.warning(this,"Wrong Name","The name of the driver can't be empty.");
        box.show();
        return;
    }

    savedChanges = false;
    if(ui->selectedCompetitorCombo->currentIndex())
    {
        this->championship->UpdateCompetitor(ui->selectedCompetitorCombo->currentIndex() - 1, ui->competitorNameBox->text().toStdString(), ui->competitorGroupCombo->currentIndex());
        this->SetUpAllTabs();
        return;
    }

    this->championship->AddCompetitor(ui->competitorNameBox->text().toStdString(), ui->competitorGroupCombo->currentIndex());
    this->SetUpAllTabs();
}


void ManageChampionshipWindow::on_deleteCompetitorButton_clicked()
{
    if(!ui->selectedCompetitorCombo->currentIndex())
    {
        QMessageBox box;
        box.warning(this,"Wrong Name","You should not be able to do that...");
        box.show();
        return;
    }

    this->championship->RemoveCompetitor(ui->selectedCompetitorCombo->currentIndex() - 1);
    this->SetUpAllTabs();
    savedChanges = false;
}

void ManageChampionshipWindow::on_randomizeNameButton_clicked()
{
    if(!this->randomNames->GetFileOpened())
    {
        QMessageBox box;
        box.setModal(true);
        box.information(this, "File Not Found", "The randomization list file does not exist. The file should be inside the resources folder and it's named should be names.txt");
        return;
    }

    ui->competitorNameBox->setText(QString::fromStdString(this->randomNames->GetRandomName()));
}

void ManageChampionshipWindow::on_selectedCompetitorCombo_currentIndexChanged(int index)
{
    ui->swapCompetitorBox->setEnabled(false);
    ui->swapCompetitorsButton->setEnabled(false);
    ui->competitorNameBox->setText("");

    if(!index) return;

    ui->swapCompetitorBox->setEnabled(true);
    ui->swapCompetitorsButton->setEnabled(true);
    ui->competitorGroupCombo->setCurrentIndex(this->championship->GetCompetitor(index - 1).GetGroupId());
}

void ManageChampionshipWindow::on_competitorGroupCombo_currentIndexChanged(int index)
{
    ui->competitorNameBox->setText("");
}

void ManageChampionshipWindow::on_swapCompetitorsButton_clicked()
{
    if(ui->selectedCompetitorCombo->currentIndex() == 0) return;
    if(ui->selectedCompetitorCombo->currentIndex() - 1 == ui->swapCompetitorBox->currentIndex()) return;

    this->championship->SwapCompetitors(ui->selectedCompetitorCombo->currentIndex() - 1, ui->swapCompetitorBox->currentIndex());
    this->SetUpAllTabs();
    this->savedChanges = false;
}


void ManageChampionshipWindow::on_reorderCompetitorsButton_clicked()
{
    QMessageBox box;
    box.setModal(true);
    auto answer = box.information(this, "Reordering Competitor Indices", "This option will reorder the internal ID of " \
                          "the competitors so they could be ordered by groups, in their name's alphabetical ordered.\n" \
                          "The change of internal Id's will affect the order of the competitors in various combo boxes or the Add Event Window.\n" \
                          "The change of internal Id wil NOT change any of the championship informations in any other way.\n" \
                          "Are you sure you want to reoreder competitors IDs?", QMessageBox::Yes|QMessageBox::No);

    if(answer == QMessageBox::Yes)
    {
        this->championship->ReorderCompetitors();
        this->SetUpAllTabs();
        this->savedChanges = false;
    }
}

void ManageChampionshipWindow::SetUpPointsTab()
{

    ui->pointsGroupCombo->blockSignals(true);
    ui->pointsGroupCombo->clear();
    ui->pointsGroupCombo->blockSignals(false);

    unsigned int count = 1;
    for(const auto& group : this->championship->GetGroupNames())
    {
        ui->pointsGroupCombo->addItem(QString::fromStdString(std::to_string(count++) + ". " + group));
    }

    this->UpdatePositionsPointsCombo();
}

void ManageChampionshipWindow::UpdatePositionsPointsCombo()
{
    ui->pointsPositionCombo->blockSignals(true);
    ui->pointsPositionCombo->clear();
    ui->pointsPositionCombo->blockSignals(false);

    if(!this->championship->GetCompetitorsCountInGroup(ui->pointsGroupCombo->currentIndex()))
    {
        ui->pointsPositionCombo->setEnabled(false);
        ui->savePointsButton->setEnabled(false);
        ui->pointsSpinBox->setValue(0);
        ui->currentPointsLabel->setText(QString::fromStdString("None"));
        return;
    }
    ui->pointsPositionCombo->setEnabled(true);
    ui->savePointsButton->setEnabled(true);


    for(unsigned int position = 1; position <= this->championship->GetCompetitorsCountInGroup(ui->pointsGroupCombo->currentIndex()); ++position)
    {
        ui->pointsPositionCombo->addItem(QString::fromStdString(utils::IntToPositionString(position)));
    }
    ui->pointsSpinBox->setValue(this->championship->GetPointsInPosition(0, ui->pointsGroupCombo->currentIndex()));
    ui->currentPointsLabel->setText(QString::fromStdString(std::to_string(this->championship->GetPointsInPosition(0, ui->pointsGroupCombo->currentIndex()))));
}

void ManageChampionshipWindow::on_savePointsButton_clicked()
{
    this->championship->SetPoints(ui->pointsSpinBox->value(), ui->pointsPositionCombo->currentIndex(), ui->pointsGroupCombo->currentIndex());
    ui->currentPointsLabel->setText(QString::fromStdString(std::to_string(this->championship->GetPointsInPosition(ui->pointsPositionCombo->currentIndex(), ui->pointsGroupCombo->currentIndex()))));
    savedChanges = false;
}

void ManageChampionshipWindow::on_pointsPositionCombo_currentIndexChanged(int index)
{
    if(this->championship->GetCompetitorsCountInGroup(ui->pointsGroupCombo->currentIndex()))
    {
        ui->pointsSpinBox->setValue(this->championship->GetPointsInPosition(index, ui->pointsGroupCombo->currentIndex()));
        ui->currentPointsLabel->setText(QString::fromStdString(std::to_string(this->championship->GetPointsInPosition(index, ui->pointsGroupCombo->currentIndex()))));
    }
}


void ManageChampionshipWindow::on_pointsGroupCombo_currentIndexChanged(int index)
{
    this->UpdatePositionsPointsCombo();
}

void ManageChampionshipWindow::SetUpGroupsTab()
{
    ui->selectedGroupCombo->blockSignals(true);
    ui->selectedGroupCombo->clear();
    ui->selectedGroupCombo->blockSignals(false);

    ui->swapGroupCombo->blockSignals(true);
    ui->swapGroupCombo->clear();
    ui->swapGroupCombo->blockSignals(false);

    ui->swapGroupCombo->setEnabled(false);
    ui->swapGroupsButton->setEnabled(false);

    ui->selectedGroupCombo->addItem("New Group");
    ui->groupNameBox->setText("");
    ui->currentGroupNameLabel->setText("");
    ui->deleteGroupButton->setEnabled(false);

    unsigned int count = 0;

    for(const auto& group : this->championship->GetGroupNames())
    {
        ui->selectedGroupCombo->addItem(QString::fromStdString(std::to_string(++count) + ". " + group));
        ui->swapGroupCombo->addItem(QString::fromStdString(std::to_string(count) + ". " + group));
    }

    if(this->championship->GetGroupCount() > 1)
    {
        ui->deleteGroupButton->setEnabled(true);
    }

}

void ManageChampionshipWindow::on_selectedGroupCombo_currentIndexChanged(int index)
{
    if(!index)
    {
        ui->swapGroupCombo->setEnabled(false);
        ui->swapGroupsButton->setEnabled(false);
        ui->groupNameBox->setText("");
        ui->currentGroupNameLabel->setText("");
        ui->deleteGroupButton->setEnabled(false);
        return;
    }

    ui->swapGroupCombo->setEnabled(true);
    ui->swapGroupsButton->setEnabled(true);
    ui->groupNameBox->setText(QString::fromStdString(this->championship->GetGroupName(index - 1)));
    ui->currentGroupNameLabel->setText(QString::fromStdString(this->championship->GetGroupName(index - 1)));

    if(this->championship->GetGroupCount() > 1) ui->deleteGroupButton->setEnabled(true);
}


void ManageChampionshipWindow::on_saveGroupButton_clicked()
{
    if(ui->groupNameBox->text().toStdString().find_first_not_of(' ') == std::string::npos || ui->groupNameBox->text().toStdString() == "")
    {
        QMessageBox box;
        box.warning(this,"Wrong Name","The name of the group can't be empty.");
        box.show();
        return;
    }

    savedChanges = false;
    if(!ui->selectedGroupCombo->currentIndex())
    {
        this->championship->AddGroup(ui->groupNameBox->text().toStdString());
        this->SetUpAllTabs();
        return;
    }

    this->championship->UpdateGroup(ui->selectedGroupCombo->currentIndex() - 1 ,ui->groupNameBox->text().toStdString());
    this->SetUpAllTabs();
}


void ManageChampionshipWindow::on_deleteGroupButton_clicked()
{
    if(!ui->selectedGroupCombo->currentIndex())
    {
        QMessageBox box;
        box.setModal(true);
        box.warning(this,"Wrong Place","You should not be here...");
        return;
    }

    QMessageBox box;
    box.setModal(true);
    auto answer = box.warning(this, "Be careful", "Deleting a group will also delete all competitors within it. Are you sure you want to proccede with deleting this group?", QMessageBox::Yes|QMessageBox::No);

    if(answer == QMessageBox::Yes)
    {
        savedChanges = false;
        this->championship->RemoveGroup(ui->selectedGroupCombo->currentIndex() - 1);
        this->SetUpAllTabs();
    }
}

void ManageChampionshipWindow::on_swapGroupsButton_clicked()
{
    if(!ui->selectedGroupCombo->currentIndex())
    {
        QMessageBox box;
        box.setModal(true);
        box.warning(this,"Wrong Place","You should not be here...");
        return;
    }

    if(ui->swapGroupCombo->currentIndex() == ui->selectedGroupCombo->currentIndex() - 1) return;

    this->championship->SwapGroups(ui->selectedGroupCombo->currentIndex() - 1, ui->swapGroupCombo->currentIndex());

    this->SetUpAllTabs();
}

void ManageChampionshipWindow::SetUpEventsTab()
{
    ui->deleteEventButton->setEnabled(false);
    ui->changeEventNameButton->setEnabled(false);
    ui->updateCompetitorButton->setEnabled(false);
    ui->swapEventsButton->setEnabled(false);
    ui->eventNameBox->setText("");

    ui->eventNameBox->setEnabled(false);
    ui->bonusPointsEventBox->setEnabled(false);
    ui->noteEventBox->setEnabled(false);
    ui->positionCompetitorCombo->setEnabled(false);

    ui->selectedEventCombo->blockSignals(true);
    ui->selectedEventCombo->setEnabled(false);
    ui->selectedEventCombo->clear();
    ui->selectedEventCombo->blockSignals(false);

    ui->selectedCompetitorEventCombo->blockSignals(true);
    ui->selectedCompetitorEventCombo->setEnabled(false);
    ui->selectedCompetitorEventCombo->clear();
    ui->selectedCompetitorEventCombo->blockSignals(false);

    ui->swapEventCombo->blockSignals(true);
    ui->swapEventCombo->clear();
    ui->swapEventCombo->setEnabled(false);
    ui->swapEventCombo->blockSignals(false);


    ui->groupEventLabel->setText("");

    if(this->championship->GetEventsCount() == 0)
    {
        return;
    }

    ui->deleteEventButton->setEnabled(true);
    ui->changeEventNameButton->setEnabled(true);
    ui->updateCompetitorButton->setEnabled(true);
    ui->swapEventsButton->setEnabled(true);

    ui->eventNameBox->setEnabled(true);
    ui->bonusPointsEventBox->setEnabled(true);
    ui->noteEventBox->setEnabled(true);
    ui->positionCompetitorCombo->setEnabled(true);

    ui->selectedEventCombo->blockSignals(true);
    ui->selectedEventCombo->setEnabled(true);
    ui->selectedEventCombo->blockSignals(false);

    ui->selectedCompetitorEventCombo->blockSignals(true);
    ui->selectedCompetitorEventCombo->setEnabled(true);
    ui->selectedCompetitorEventCombo->blockSignals(false);

    ui->swapEventCombo->blockSignals(true);
    ui->swapEventCombo->setEnabled(true);
    ui->swapEventCombo->blockSignals(false);

    ui->selectedEventCombo->blockSignals(true);
    unsigned int count = 0;
    for(const auto& eventName : this->championship->GetAllEventNames())
    {
        ui->selectedEventCombo->addItem(QString::fromStdString(std::to_string(++count) + ". " + eventName));
        ui->swapEventCombo->addItem(QString::fromStdString(std::to_string(count) + ". " + eventName));
    }
    ui->selectedEventCombo->setCurrentIndex(0);
    ui->selectedEventCombo->blockSignals(false);

    ui->positionCompetitorCombo->blockSignals(true);
    ui->positionCompetitorCombo->clear();

    ui->selectedCompetitorEventCombo->blockSignals(true);
    for(unsigned int index=0;index<this->championship->GetCompetitorsCount();++index)
    {
        ui->selectedCompetitorEventCombo->addItem(QString::fromStdString(this->championship->GetCompetitor(index).GetName()));
        ui->positionCompetitorCombo->addItem(QString::fromStdString(utils::IntToPositionString(index + 1)));
    }
    ui->selectedCompetitorEventCombo->setCurrentIndex(0);
    ui->selectedCompetitorEventCombo->blockSignals(false);
    ui->positionCompetitorCombo->blockSignals(false);

    UpdateEventTabElements();
}

void ManageChampionshipWindow::UpdateEventTabElements()
{
    Event currentEvent = this->championship->GetEvent(ui->selectedEventCombo->currentIndex());
    unsigned int currentCompetitorIndex = ui->selectedCompetitorEventCombo->currentIndex();

    ui->noteEventBox->setPlainText(QString::fromStdString(currentEvent.GetNoteOfDriver(currentCompetitorIndex)));
    ui->groupEventLabel->setText(QString::fromStdString("Group: " + this->championship->GetGroupName(this->championship->GetCompetitor(currentCompetitorIndex).GetGroupId())));
    ui->bonusPointsEventBox->setValue(currentEvent.GetBonusPointsOfDriver(currentCompetitorIndex));
    ui->positionCompetitorCombo->setCurrentIndex(currentEvent.GetPositionOfDriver(currentCompetitorIndex));
    ui->eventNameBox->setText(QString::fromStdString(currentEvent.GetName()));
    ui->isDisqualifiedCheck->setChecked(currentEvent.DisqualifiedOfDriver(currentCompetitorIndex));
    ui->bonusPointsEventBox->setValue(currentEvent.GetBonusPointsOfDriver(currentCompetitorIndex));
}

void ManageChampionshipWindow::on_selectedEventCombo_currentIndexChanged(int index)
{
    this->UpdateEventTabElements();
}

void ManageChampionshipWindow::on_selectedCompetitorEventCombo_currentIndexChanged(int index)
{
    this->UpdateEventTabElements();
}

void ManageChampionshipWindow::on_deleteEventButton_clicked()
{
    this->championship->RemoveEvent(ui->selectedEventCombo->currentIndex());

    QMessageBox info;
    info.setModal(true);
    info.information(this, "Deleted Event", "The event " + ui->selectedEventCombo->currentText() + " has been deleted.");

    this->SetUpAllTabs();
    this->savedChanges=false;
}


void ManageChampionshipWindow::on_changeEventNameButton_clicked()
{
    this->championship->ChangeEventName(ui->selectedEventCombo->currentIndex(), ui->eventNameBox->text().toStdString());

    QMessageBox info;
    info.setModal(true);
    info.information(this, "Changed name of event", "The event formerly known as " + ui->selectedEventCombo->currentText() + " has been renamed to:\n" + ui->eventNameBox->text());

    this->SetUpAllTabs();
    this->savedChanges=false;
}


void ManageChampionshipWindow::on_updateCompetitorButton_clicked()
{
    this->championship->UpdateEventCompetitor(ui->selectedEventCombo->currentIndex(), ui->positionCompetitorCombo->currentIndex(), ui->positionCompetitorCombo->currentIndex(), ui->bonusPointsEventBox->value(), ui->noteEventBox->toPlainText().toStdString(), ui->isDisqualifiedCheck->isChecked());

    QMessageBox info;
    info.setModal(true);
    info.information(this, "Updated Competitor", "The competitor " + ui->selectedCompetitorEventCombo->currentText() + " has been updated.");;

    this->SetUpAllTabs();
    this->savedChanges=false;
}

void ManageChampionshipWindow::on_swapEventsButton_clicked()
{
    if(ui->selectedEventCombo->currentIndex() == ui->swapEventCombo->currentIndex()) return;

    this->championship->SwapEvents(ui->selectedEventCombo->currentIndex(), ui->swapEventCombo->currentIndex());

    this->SetUpAllTabs();
    this->savedChanges=false;
}

void ManageChampionshipWindow::reject()
{
    if(!savedChanges)
    {
        QMessageBox box;
        box.setModal(true);
        box.warning(this, "You have unsaved chages", "There are unsaved changes. Are you sure you want to quit?", QMessageBox::Yes|QMessageBox::No);

        if (box.result() == QMessageBox::Yes) {
            QDialog::reject();
        }
    }
    QDialog::reject();
}

