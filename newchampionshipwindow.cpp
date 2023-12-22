#include "newchampionshipwindow.h"
#include "ui_newchampionshipwindow.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <QMessageBox>

#include "utils.h"

NewChampionshipWindow::NewChampionshipWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewChampionshipWindow)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    ui->buttonBox->addButton("Add", QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton("Close", QDialogButtonBox::RejectRole);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

NewChampionshipWindow::~NewChampionshipWindow()
{
    delete ui;
}

void NewChampionshipWindow::on_buttonBox_accepted()
{

    if(ui->nameBox->text().toStdString().empty() || ui->nameBox->text().toStdString().find_first_of(".*:/\\\"\'<>,|?") != std::string::npos || ui->nameBox->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {
        QMessageBox box;
        box.warning(this,"Wrong Name","The Name can't be empty or contain the following symbols:\n.*:/\\\"\'<>,|?");
        box.show();
        return;
    }

    unsigned int count=0;
    auto newFilePath = std::filesystem::current_path() / "saves" / (ui->nameBox->text().toStdString() + ".qad");
    while(std::filesystem::exists(newFilePath))
    {
        newFilePath = std::filesystem::current_path() / "saves" / (ui->nameBox->text().toStdString() + " (" + std::to_string(++count) + ")" + ".qad");
    }
    std::ofstream outFile(newFilePath);

    outFile << ui->nameBox->text().toStdString()<<std::endl;
    outFile << 0 << ' ' << 0 << ' ' << 1 << std::endl;
    outFile << "The championship's description." << std::endl;
    outFile << utils::endOfNoteChar <<std::endl;
    outFile << "Class" << std::endl;
    outFile << 0 ;

    outFile.close();

    this->close();
}


void NewChampionshipWindow::on_buttonBox_rejected()
{
    this->close();
}

