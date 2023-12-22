#ifndef MANAGECHAMPIONSHIPWINDOW_H
#define MANAGECHAMPIONSHIPWINDOW_H

#include <QDialog>
#include "championship.h"
#include "randomnames.h"

namespace Ui {
class ManageChampionshipWindow;
}

class ManageChampionshipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManageChampionshipWindow(Championship* championship ,QWidget *parent = nullptr);
    ~ManageChampionshipWindow();

private:
    void SetUpAllTabs();

    void SetUpCompetitorsTab();
    void SetUpPointsTab();
    void SetUpGroupsTab();
    void SetUpEventsTab();

    void UpdatePositionsPointsCombo();
    void UpdateEventTabElements();

    void reject();

private slots:
    void on_changeChampionshipNameButton_clicked();

    void on_saveAllButton_clicked();

    void on_selectedDriverBox_currentIndexChanged(int index);

    void on_saveCompetitorButton_clicked();

    void on_deleteCompetitorButton_clicked();

    void on_savePointsButton_clicked();

    void on_pointsPositionCombo_currentIndexChanged(int index);

    void on_pointsGroupCombo_currentIndexChanged(int index);

    void on_selectedGroupCombo_currentIndexChanged(int index);

    void on_saveGroupButton_clicked();

    void on_deleteGroupButton_clicked();

    void on_selectedCompetitorCombo_currentIndexChanged(int index);

    void on_competitorGroupCombo_currentIndexChanged(int index);

    void on_selectedEventCombo_currentIndexChanged(int index);

    void on_selectedCompetitorEventCombo_currentIndexChanged(int index);

    void on_deleteEventButton_clicked();

    void on_changeEventNameButton_clicked();

    void on_updateCompetitorButton_clicked();

    void on_randomizeNameButton_clicked();

private:
    Ui::ManageChampionshipWindow *ui;
    Championship *championship;
    Championship *referenceChampionship;
    bool savedChanges;
    RandomNames *randomNames;
};

#endif // MANAGECHAMPIONSHIPWINDOW_H
