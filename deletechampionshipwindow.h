#ifndef DELETECHAMPIONSHIPWINDOW_H
#define DELETECHAMPIONSHIPWINDOW_H

#include <QDialog>
//#include <string>

namespace Ui {
class DeleteChampionshipWindow;
}

class DeleteChampionshipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteChampionshipWindow(const std::string& name = "", QWidget *parent = nullptr);
    ~DeleteChampionshipWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DeleteChampionshipWindow *ui;
};

#endif // DELETECHAMPIONSHIPWINDOW_H
