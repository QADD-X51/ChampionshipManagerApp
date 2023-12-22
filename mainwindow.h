#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "savesreader.h"
#include "newchampionshipwindow.h"
#include "deletechampionshipwindow.h"
#include "championshipwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newChampButton_clicked();

    void on_deleteButton_clicked();

    void on_openButton_clicked();

private:
    void SetUpList();

private:
    Ui::MainWindow *ui;
    SavesReader* savesReader;
    ChampionshipWindow* mainWindow;
};
#endif // MAINWINDOW_H
