#ifndef CHAMPIONSHIPWINDOW_H
#define CHAMPIONSHIPWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QList>
#include <QString>

#include "championship.h"
#include "addeventwindow.h"

namespace Ui {
class ChampionshipWindow;
}

class ChampionshipWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChampionshipWindow(const std::string& path, QWidget *parent = nullptr);
    ~ChampionshipWindow();

private slots:
    void on_addEventButton_clicked();

    void on_manageButton_clicked();

    void on_saveButton_clicked();

    void on_eventsComboBox_currentIndexChanged(int index);

    void on_prevGroupButton_clicked();

    void on_nextGroupButton_clicked();

    void on_reorderCheck_stateChanged(int arg1);

    void on_resultsTable_cellClicked(int row, int column);

private:
    void DisplayData();
    void SetUpTable();
    void ClearCompetitorNotes();

private:
    Ui::ChampionshipWindow *ui;
    Championship *championship;
    unsigned int currentGroupSelected;
    QList<QString*> competitorsNotes;
    QList<QTableWidgetItem*> tableCells;

};

#endif // CHAMPIONSHIPWINDOW_H
