#include "deletechampionshipwindow.h"
#include "ui_deletechampionshipwindow.h"

DeleteChampionshipWindow::DeleteChampionshipWindow(const std::string& name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteChampionshipWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->buttonBox->addButton("Delete", QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton("Close", QDialogButtonBox::RejectRole);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    ui->label->setText(QString::fromStdString("Are you sure you want to PERMANENTLLY delete this chamiponship?\n" + name));
}

DeleteChampionshipWindow::~DeleteChampionshipWindow()
{
    delete ui;
}

void DeleteChampionshipWindow::on_buttonBox_accepted()
{
    this->close();
}


void DeleteChampionshipWindow::on_buttonBox_rejected()
{
    this->close();
}

