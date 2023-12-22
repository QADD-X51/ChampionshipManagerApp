#ifndef ADDEVENTWINDOW_H
#define ADDEVENTWINDOW_H

#include <QDialog>
#include <QList>

#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QSpacerItem>

#include "championship.h"

namespace Ui {
class AddEventWindow;
}

class AddEventWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddEventWindow(Championship* championshipRef, QWidget *parent = nullptr);
    ~AddEventWindow();


private slots:
    void on_addEventeButton_clicked();

private:
    void SetUpScrollArea();

    std::vector<unsigned int> MakePositionsVector() const;
    void BuildEvent(const std::vector<unsigned int>& positionArray) const;

    Ui::AddEventWindow *ui;
    Championship* championshipRef;

    QList<QLabel*> countLabels;
    QList<QLabel*> competitorNameLabels;
    QList<QLabel*> groupNameLabels;
    QList<QComboBox*> positionCombos;
    QList<QSpinBox*> bonusPointsSpins;
    QList<QTextEdit*> noteBoxs;
    QList<QCheckBox*> disqualifiedChecks;
    QList<QComboBox*> minPositionCombos;
    QList<QComboBox*> maxPositionCombos;

};

#endif // ADDEVENTWINDOW_H
