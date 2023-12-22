#ifndef NEWCHAMPIONSHIPWINDOW_H
#define NEWCHAMPIONSHIPWINDOW_H

#include <QDialog>

namespace Ui {
class NewChampionshipWindow;
}

class NewChampionshipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewChampionshipWindow(QWidget *parent = nullptr);
    ~NewChampionshipWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::NewChampionshipWindow *ui;

};

#endif // NEWCHAMPIONSHIPWINDOW_H
