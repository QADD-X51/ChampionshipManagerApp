#include "championshipwindow.h"
#include "ui_championshipwindow.h"

#include <QMessageBox>
#include "managechampionshipwindow.h"

ChampionshipWindow::ChampionshipWindow(const std::string& path, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChampionshipWindow)
{
    ui->setupUi(this);

    this->championship = new Championship(path);

    ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    DisplayData();
}

void ChampionshipWindow::DisplayData()
{
    ui->championshipNameLabel->setText(QString::fromStdString(this->championship->GetName()));
    this->currentGroupSelected = 0;
    ui->staticClassLabel->hide();
    ui->classLabel->hide();
    ui->prevGroupButton->hide();
    ui->nextGroupButton->hide();

    ui->classLabel->setText("All");

    if(this->championship->GetGroupCount() > 1)
    {
        ui->staticClassLabel->show();
        ui->classLabel->show();
        ui->prevGroupButton->show();
        ui->nextGroupButton->show();
    }

    ui->eventsComboBox->blockSignals(true);
    ui->eventsComboBox->clear();
    ui->eventsComboBox->addItem("Overview");
    ui->eventsComboBox->blockSignals(false);

    unsigned int count = 1;
    for(const auto& name : this->championship->GetAllEventNames())
    {
        ui->eventsComboBox->addItem(QString::fromStdString(std::to_string(count++) + " - " + name));
    }

    SetUpTable();
}

void ChampionshipWindow::SetUpTable()
{
    ui->resultsTable->blockSignals(true);
    ui->resultsTable->clear();
    ui->resultsTable->blockSignals(false);
    this->tableCells.clear();

    this->ClearCompetitorNotes();
    ui->descriptionLabel->setText(QString::fromStdString(this->championship->GetDescription()));

    if(ui->eventsComboBox->currentIndex() == 0)
    {
        QStringList headers;
        headers << "Position" << "Name" << "Points" << "Group";
        ui->resultsTable->setColumnCount(4);
        ui->resultsTable->setHorizontalHeaderLabels(headers);

        ui->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        std::vector<std::tuple<std::string,std::string,int,std::string>> results = this->championship->CalculateOverviewPoints(this->currentGroupSelected);

        ui->resultsTable->setRowCount(results.size());

        for(unsigned int row = 0; row<ui->resultsTable->rowCount();++row)
        {
            for(unsigned int column=0;column<ui->resultsTable->columnCount();++column)
            {
                QTableWidgetItem* cell = new QTableWidgetItem;
                this->tableCells.append(cell);

                if(column == 0)
                {
                    cell->setText(QString::fromStdString(std::get<0>(results.at(row))));
                    ui->resultsTable->setItem(row, column, cell);
                    continue;
                }

                if(column == 1)
                {
                    cell->setText(QString::fromStdString(std::get<1>(results.at(row))));
                    ui->resultsTable->setItem(row, column, cell);
                    continue;
                }

                if(column == 2)
                {
                    cell->setText(QString::fromStdString(std::to_string(std::get<2>(results.at(row)))));
                    ui->resultsTable->setItem(row, column, cell);
                    continue;
                }

                cell->setText(QString::fromStdString(std::get<3>(results.at(row))));
                ui->resultsTable->setItem(row, column, cell);

            }
        }

        return;
    }


    QStringList headers;
    headers << "Position" << "Name" << "Points" <<"Bonus Points" << "Total Points" << "Group" << "Disqalified";
    ui->resultsTable->setColumnCount(7);
    ui->resultsTable->setHorizontalHeaderLabels(headers);

    ui->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    std::vector<std::tuple<std::string,std::string,int,int,int,std::string, std::string, std::string>> results = this->championship->CalculateEventPoints(ui->eventsComboBox->currentIndex() - 1, this->currentGroupSelected, ui->reorderCheck->isChecked());

    ui->resultsTable->setRowCount(results.size());

    for(unsigned int row = 0; row<ui->resultsTable->rowCount();++row)
    {
        for(unsigned int column=0;column<ui->resultsTable->columnCount();++column)
        {
            QTableWidgetItem* cell = new QTableWidgetItem;
            this->tableCells.append(cell);

            switch(column)
            {
            case 0:
                cell->setText(QString::fromStdString(std::get<0>(results.at(row))));
                break;
            case 1:
                cell->setText(QString::fromStdString(std::get<1>(results.at(row))));
                break;
            case 2:
                cell->setText(QString::fromStdString(std::to_string(std::get<2>(results.at(row)))));
                break;
            case 3:
                cell->setText(QString::fromStdString(std::to_string(std::get<3>(results.at(row)))));
                break;
            case 4:
                cell->setText(QString::fromStdString(std::to_string(std::get<4>(results.at(row)))));
                break;
            case 5:
                cell->setText(QString::fromStdString(std::get<5>(results.at(row))));
                break;
            case 6:
                cell->setText(QString::fromStdString(std::get<7>(results.at(row))));
            }

            ui->resultsTable->setItem(row, column, cell);

        }
        this->competitorsNotes.push_back(new QString(QString::fromStdString(std::get<6>(results.at(row)))));
    }
}

ChampionshipWindow::~ChampionshipWindow()
{
    delete ui;
    delete championship;
}

void ChampionshipWindow::ClearCompetitorNotes()
{
    for(unsigned int index=0;index<this->competitorsNotes.size();++index)
    {
        delete this->competitorsNotes.at(index);
    }
    this->competitorsNotes.clear();
}

void ChampionshipWindow::on_addEventButton_clicked()
{
    if(!this->championship->GetCompetitorsCount())
    {
        QMessageBox::warning(this, "Create Drivers", "You should add some drivers before you start adding events.");
        return;
    }

    AddEventWindow newWindow(this->championship);
    newWindow.setModal(true);
    newWindow.exec();
    this->DisplayData();
}


void ChampionshipWindow::on_manageButton_clicked()
{
    ManageChampionshipWindow newWindow(this->championship);
    newWindow.setModal(true);
    newWindow.exec();
    this->DisplayData();
}


void ChampionshipWindow::on_saveButton_clicked()
{
    this->championship->SaveToFile();
    QMessageBox::information(this, "File Saved", "Championship Saved");
}


void ChampionshipWindow::on_eventsComboBox_currentIndexChanged(int index)
{
    ui->reorderCheck->setEnabled(true);
    if(index == 0) ui->reorderCheck->setEnabled(false);
    this->SetUpTable();
}


void ChampionshipWindow::on_prevGroupButton_clicked()
{
    if(this->currentGroupSelected != 0)
    {
        --this->currentGroupSelected;
        std::string newGroupName = this->currentGroupSelected == 0 ? "All" : this->championship->GetGroupName(this->currentGroupSelected - 1);
        ui->classLabel->setText(QString::fromStdString(newGroupName));
        this->SetUpTable();
        return;
    }

    this->currentGroupSelected = this->championship->GetGroupCount();
    ui->classLabel->setText(QString::fromStdString(this->championship->GetGroupName(this->currentGroupSelected - 1)));
    this->SetUpTable();
}


void ChampionshipWindow::on_nextGroupButton_clicked()
{
    if(this->currentGroupSelected < this->championship->GetGroupCount())
    {
        ++this->currentGroupSelected;
        ui->classLabel->setText(QString::fromStdString(this->championship->GetGroupName(this->currentGroupSelected - 1)));
        this->SetUpTable();
        return;
    }

    this->currentGroupSelected = 0;
    ui->classLabel->setText("All");
    this->SetUpTable();
}


void ChampionshipWindow::on_reorderCheck_stateChanged(int arg1)
{
    this->SetUpTable();
}


void ChampionshipWindow::on_resultsTable_cellClicked(int row, int column)
{
    if(ui->eventsComboBox->currentIndex() == 0) return;

    ui->descriptionLabel->setText(*this->competitorsNotes.at(row));
}

