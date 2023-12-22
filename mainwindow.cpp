#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->savesReader = new SavesReader();
    this->savesReader->ReadSaves();
    ui->setupUi(this);

    this->SetUpList();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete savesReader;
}

void MainWindow::SetUpList()
{
    ui->deleteButton->setEnabled(false);
    ui->openButton->setEnabled(false);
    for(const auto& save : savesReader->GetSaves())
    {
        ui->championshipList->addItem(QString::fromStdString(save.GetName() + " ( Events: " + std::to_string(save.GetEventsCount()) + "  Competitors: " + std::to_string(save.GetCompetitorsCount()) + " )"));
    }
    if(savesReader->GetSavesCount())
    {
        ui->deleteButton->setEnabled(true);
        ui->openButton->setEnabled(true);
    }
}

void MainWindow::on_newChampButton_clicked()
{
    NewChampionshipWindow newDialog;
    newDialog.setModal(true);
    newDialog.exec();
    if(newDialog.result() == QDialog::Accepted)
    {
        this->savesReader->DeleteSaves();
        this->savesReader->ReadSaves();
        ui->championshipList->clear();
        this->SetUpList();
    }
}


void MainWindow::on_deleteButton_clicked()
{
    //std::cout<<ui->championshipList->currentIndex().row()<<std::endl;
    DeleteChampionshipWindow newDialog(this->savesReader->GetSaveName(ui->championshipList->currentIndex().row()));
    newDialog.setModal(true);
    newDialog.exec();
    if(newDialog.result() == QDialog::Accepted)
    {
        this->savesReader->DeleteSaveFile(ui->championshipList->currentIndex().row());
        this->savesReader->DeleteSaves();
        this->savesReader->ReadSaves();
        ui->championshipList->clear();
        this->SetUpList();
    }

}


void MainWindow::on_openButton_clicked()
{
    mainWindow = new ChampionshipWindow(savesReader->GetPathOfSave(ui->championshipList->currentIndex().row()));
    mainWindow->show();
    this->close();
}

