#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include<iostream>
#include "participant.h"
#include "championship.h"
using namespace std;

namespace Ui {
class MainWindow;
}
class AddParticipant;
class QActionGroup;
class AddOrganization;
class AddChampionship;
class AddAgeCategory;
class AddWeightCategory;
class AddExperienceCategory;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onAddItem();
    void onAddOrganization();
    void onAddAgeCategory();
    void onAddWeightCategory();
    void onAddChampionship();
    void onAddExperienceCategory();
    void onRefreshDB();
    void onNextRound();
    int calculateWeightId(int weight);
    void iterateParticipants();
    void iterateChampionship();
    int calculateAgeId(int age);
    void onTableActionsTriggered(QAction *action);


private:
    void setupModel();
    void insertQuery(const QString &id, const QString &firstName, const QString &lastName,
                     const QString &age, const QString &weight, const QString &experience,
                     const QString &organization_id, const QString &championship_id);
    void insertQuery1(const QString &minAge, const QString &maxAge, const QString &age_id);
    void insertQuery(const QString &exp, const QString &experience_id);
    void insertQuery2(const QString &min_bound, const QString &max_bound, const QString &weight_id);
    void insertQuery(const QString &name, const QString &info, const QString &organization_id);
    void insertQuery(const QString &name, const QString &location, const QString &championship_id,
                                  const QString &qualification_score, const QString &quater_finals_score,
                                  const QString &semifinals_score, const QString &finale_score);
    void selectQuery();
    void readSettings(QString &hostName, QString &databaseName, QString &userName, QString &password);
    Championship& getChampionshipById(int id);
    void simulateFights();
    void fight(Participant participanti[],int players, int score, int stage);
    void addParticipant(Participant participanti[],Participant participanti1[], QString fn, QString ln, int id, int j, int i);
    void clearScore();

private:
    Ui::MainWindow *ui;

    QActionGroup *m_tableActions;
    AddParticipant *m_AddParticipant;
    AddOrganization *m_addOrganization;
    AddChampionship *m_addChampionship;
    AddAgeCategory *m_addAgeCategory;
    AddExperienceCategory *m_addExperienceCategory;
    AddWeightCategory *m_addWeightCategory;
    QSqlTableModel *m_personsModel;
    QSqlTableModel *m_ageModel;
    QSqlTableModel *m_experienceModel;
    QSqlTableModel *m_weightModel;
    QSqlTableModel *m_organizationModel;
    QSqlTableModel *m_championshipModel;
    QSqlRelationalTableModel *m_simModel;
    QSqlQueryModel *m_participantJoinChampModel;
    QSqlDatabase m_db;
    Championship championship[100];
     Participant participanti[100];
     Participant qfParticipanti[100];
     Participant sfParticipanti[100];
     Participant fParticipanti[100];

};

#endif // MAINWINDOW_H
