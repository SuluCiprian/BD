#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addparticipant.h"
#include "addorganization.h"
#include "addagecategory.h"
#include "addchampionship.h"
#include "addweightcategory.h"
#include "addexperiencecategory.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSettings>
#include <QActionGroup>
#include <QItemSelectionModel>
#include <QDebug>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tableActions = new QActionGroup(this);
    m_tableActions->addAction(ui->actionTables);
    m_tableActions->addAction(ui->actionOrganizations);
    m_tableActions->addAction(ui->actionChampionship);
    m_tableActions->addAction(ui->actionAge);
    m_tableActions->addAction(ui->actionWeight);
    m_tableActions->addAction(ui->actionExperience);
    m_tableActions->addAction(ui->actionJoin_Table);
    //m_tableActions->addAction(ui->actionNext_Round);

    connect(m_tableActions, &QActionGroup::triggered, this, &MainWindow::onTableActionsTriggered);
    connect(ui->actionAdd_Item, &QAction::triggered, this, &MainWindow::onAddItem);
    connect(ui->actionAdd_Organization, &QAction::triggered, this, &MainWindow::onAddOrganization);
    connect(ui->actionAdd_Championship, &QAction::triggered, this, &MainWindow::onAddChampionship);
    connect(ui->actionAdd_Age_Category, &QAction::triggered, this, &MainWindow::onAddAgeCategory);
    connect(ui->actionAdd_Weight_Category, &QAction::triggered, this, &MainWindow::onAddWeightCategory);
    connect(ui->actionAdd_Experience_Category, &QAction::triggered, this, &MainWindow::onAddExperienceCategory);

    m_AddParticipant = new AddParticipant(this);
    m_addOrganization = new AddOrganization(this);
    m_addAgeCategory = new AddAgeCategory(this);
    m_addWeightCategory = new AddWeightCategory(this);
    m_addChampionship = new AddChampionship(this);
    m_addExperienceCategory = new AddExperienceCategory(this);

    QString hostName = "baasu.db.elephantsql.com";
    QString databaseName = "wakisiwz";
    QString userName = "wakisiwz";
    QString password = "jfQFEk934qEUUF5K7SIAs7oLOhze4N_u";

    readSettings(hostName, databaseName, userName, password);
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(hostName);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(userName);
    m_db.setPassword(password);
    bool ok = m_db.open();
    if(!ok)
    {
        ui->statusBar->showMessage(tr("Database not connected!"));
        cout<<m_db.lastError().databaseText().toStdString();
        cout<< m_db.lastError().driverText().toStdString();
    }
    else
    {
        ui->statusBar->showMessage(tr("Database connected!"));
        iterateChampionship();
        iterateParticipants();
        clearScore();
        simulateFights();
    }
    setupModel();
    ui->tablePersons->setModel(m_personsModel);
    ui->tableSim->setModel(m_participantJoinChampModel);
    ui->tableOrganization->setModel(m_organizationModel);
    ui->tableChampionship->setModel(m_championshipModel);
    ui->tableAge->setModel(m_ageModel);
    ui->tableWeight->setModel(m_weightModel);
    ui->tableExperience->setModel(m_experienceModel);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::onRefreshDB);
    connect(ui->actionNext_Round, &QAction::triggered, this, &MainWindow::onNextRound);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings(QString &hostName, QString &databaseName, QString &userName, QString &password)
{
    qDebug() << "App dir path " << qApp->applicationDirPath();
    QSettings s(qApp->applicationDirPath() + "/DBTests.conf", QSettings::IniFormat);
    hostName = s.value("HOSTNAME").toString();
    databaseName = s.value("DATABASENAME").toString();
    userName = s.value("USERNAME").toString();
    password = s.value("PASSWORD").toString();
    qDebug() << "Settings file name " << s.fileName();
}

void MainWindow::onAddItem()
{
    m_AddParticipant->setType(AddParticipant::AddType::ADD_PERSON);
    int r = m_AddParticipant->exec();
    if(r == QDialog::Accepted)
    {
        QString firstName;
        QString lastName;
        QString id;
        QString age;
        QString weight;
        QString experience;
        QString organization_id;
        QString championship_id;

        m_AddParticipant->data(firstName, lastName, id, age, weight, experience,organization_id,championship_id);
        insertQuery(id, firstName, lastName, age, weight, experience,organization_id,championship_id);
    }
}

void MainWindow::onAddOrganization()
{
    m_addOrganization->setType(AddOrganization::AddType::ADD_PERSON);
    int r = m_addOrganization->exec();
    if(r == QDialog::Accepted)
    {
        QString name;
        QString info;
        QString organization_id;
        m_addOrganization->data(name, info, organization_id);
        insertQuery(name, info, organization_id);
    }
}
void MainWindow::onAddAgeCategory()
{
    m_addAgeCategory->setType(AddAgeCategory::AddType::ADD_PERSON);
    int r = m_addAgeCategory->exec();
    if(r == QDialog::Accepted)
    {
        QString minAge;
        QString maxAge;
        QString age_id;
        m_addAgeCategory->data(minAge, maxAge, age_id);
        insertQuery1(minAge, maxAge, age_id);
    }
}

void MainWindow::onAddExperienceCategory()
{
    m_addExperienceCategory->setType(AddExperienceCategory::AddType::ADD_PERSON);
    int r = m_addExperienceCategory->exec();
    if(r == QDialog::Accepted)
    {
        QString exp;
        QString experience_id;
        m_addExperienceCategory->data(exp, experience_id);
        insertQuery(exp, experience_id);
    }
}

void MainWindow::onAddWeightCategory()
{
    m_addWeightCategory->setType(AddWeightCategory::AddType::ADD_PERSON);
    int r = m_addWeightCategory->exec();
    if(r == QDialog::Accepted)
    {
        QString min_bound;
        QString max_bound;
        QString weight_id;
        m_addWeightCategory->data(min_bound, max_bound, weight_id);
        insertQuery2(min_bound, max_bound, weight_id);
    }
}

void MainWindow::onAddChampionship()
{
    m_addChampionship->setType(AddChampionship::AddType::ADD_PERSON);
    int r = m_addChampionship->exec();
    if(r == QDialog::Accepted)
    {
        QString name;
        QString location;
        QString championship_id;
        QString qualification_score;
        QString quater_finals_score;
        QString semifinals_score;
        QString finale_score;
        m_addChampionship->data(name,  location,  championship_id,
                                qualification_score, quater_finals_score,
                                semifinals_score, finale_score);
        insertQuery(name,  location,  championship_id,
                    qualification_score, quater_finals_score,
                    semifinals_score, finale_score);
    }
}

void MainWindow::onRefreshDB()
{
    selectQuery();
}

void MainWindow::setupModel()
{
    // model Participanti
    ui->tablePersons->setSelectionMode(QAbstractItemView::SingleSelection);
    m_personsModel = new QSqlTableModel(this, m_db);
    m_personsModel->setTable("Participanti");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_personsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_personsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_personsModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m_personsModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m_personsModel->select();

    // model oraganizatii
    ui->tableOrganization->setSelectionMode(QAbstractItemView::SingleSelection);
    m_organizationModel = new QSqlTableModel(this, m_db);
    m_organizationModel->setTable("organization");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_organizationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_organizationModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_organizationModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_organizationModel->setHeaderData(2, Qt::Horizontal, tr("Info"));
    m_organizationModel->select();

    // model categorii varsta
    ui->tableAge->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ageModel = new QSqlTableModel(this, m_db);
    m_ageModel->setTable("age");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_ageModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_ageModel->setHeaderData(0, Qt::Horizontal, tr("Age Id"));
    m_ageModel->setHeaderData(1, Qt::Horizontal, tr("Min Age"));
    m_ageModel->setHeaderData(2, Qt::Horizontal, tr("Max Age"));
    m_ageModel->select();

    // model categorii greutate
    ui->tableWeight->setSelectionMode(QAbstractItemView::SingleSelection);
    m_weightModel = new QSqlTableModel(this, m_db);
    m_weightModel->setTable("weight");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_weightModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_weightModel->setHeaderData(0, Qt::Horizontal, tr("Weight Id"));
    m_weightModel->setHeaderData(1, Qt::Horizontal, tr("Min Weight"));
    m_weightModel->setHeaderData(2, Qt::Horizontal, tr("Max Weight"));
    m_weightModel->select();

    // model categorii experienta
    ui->tableExperience->setSelectionMode(QAbstractItemView::SingleSelection);
    m_experienceModel = new QSqlTableModel(this, m_db);
    m_experienceModel->setTable("experience");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_experienceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_experienceModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_experienceModel->setHeaderData(1, Qt::Horizontal, tr("Experience"));
    m_experienceModel->select();

    // model campionat
    ui->tableChampionship->setSelectionMode(QAbstractItemView::SingleSelection);
    m_championshipModel = new QSqlTableModel(this, m_db);
    m_championshipModel->setTable("championship");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_championshipModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_championshipModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_championshipModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_championshipModel->setHeaderData(2, Qt::Horizontal, tr("Location"));
    m_championshipModel->setHeaderData(3, Qt::Horizontal, tr("Qualificuation Score"));
    m_championshipModel->setHeaderData(4, Qt::Horizontal, tr("Quaterfinal Score"));
    m_championshipModel->setHeaderData(5, Qt::Horizontal, tr("Semifinal Score"));
    m_championshipModel->setHeaderData(6, Qt::Horizontal, tr("Final Score"));
    m_championshipModel->select();


    m_participantJoinChampModel = new QSqlQueryModel(this);
    QSqlQuery query("SELECT Participanti.first_name, Participanti.last_name, championship.qualification_score, championship.quater_finals_score, championship.semifinals_score, championship.finale_score FROM Participanti INNER JOIN championship ON Participanti.championship_id=championship.championship_id");
    m_participantJoinChampModel->setQuery(query);
    m_participantJoinChampModel->setHeaderData(0, Qt::Horizontal, tr("First Name"));
    m_participantJoinChampModel->setHeaderData(1, Qt::Horizontal, tr("Last Name"));
    m_participantJoinChampModel->setHeaderData(2, Qt::Horizontal, tr("Qualificuation Score"));
    m_participantJoinChampModel->setHeaderData(3, Qt::Horizontal, tr("Quaterfinal Score"));
    m_participantJoinChampModel->setHeaderData(4, Qt::Horizontal, tr("Semifinal Score"));
    m_participantJoinChampModel->setHeaderData(5, Qt::Horizontal, tr("Final Score"));
    //m_participantJoinChampModel->select();
}

// introduce participant
void MainWindow::insertQuery(const QString &id, const QString &firstName, const QString &lastName,
                             const QString &age, const QString &weight, const QString &experience,
                              const QString &organization_id, const QString &championship_id)
{
    QSqlField idField("participant_id", QVariant::Int);
    QSqlField firstNameField("first_name", QVariant::String);
    QSqlField lastNameField("last_name", QVariant::String);
    QSqlField ageField("age", QVariant::Int);
    QSqlField weightField("weight", QVariant::Int);
    QSqlField experienceField("experience", QVariant::String);
    QSqlField weightIdField("weight_id", QVariant::Int);
    QSqlField ageIdField("age_id", QVariant::Int);
    QSqlField organizationIdField("organization_id", QVariant::Int);
    QSqlField championshipIdField("championship_id", QVariant::Int);

    idField.setValue(id);
    firstNameField.setValue(firstName);
    lastNameField.setValue(lastName);
    ageField.setValue(age);
    weightField.setValue(weight);
    experienceField.setValue(experience);
    int weightId = calculateWeightId(weightField.value().toInt());
    int ageId = calculateAgeId(ageField.value().toInt());
    weightIdField.setValue(weightId);
    ageIdField.setValue(ageId);
    organizationIdField.setValue(organization_id);
    championshipIdField.setValue(championship_id);

    QSqlRecord record;
    record.append(idField);
    record.append(firstNameField);
    record.append(lastNameField);
    record.append(ageField);
    record.append(weightField);
    record.append(experienceField);
    record.append(weightIdField);
    record.append(ageIdField);
    record.append(organizationIdField);
    record.append(championshipIdField);

    m_personsModel->insertRecord(-1, record);
    if(!m_personsModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
         qDebug()<< m_personsModel->lastError();
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}


// introduce organiatie
void MainWindow::insertQuery(const QString &name, const QString &info, const QString &organization_id)
{
    QSqlField idField("organization_id", QVariant::Int);
    QSqlField nameField("name", QVariant::String);
    QSqlField infoField("info", QVariant::String);
    idField.setValue(organization_id);
    nameField.setValue(name);
    infoField.setValue(info);
    QSqlRecord record;
    record.append(idField);
    record.append(nameField);
    record.append(infoField);
    m_organizationModel->insertRecord(-1, record);
    if(!m_organizationModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

// introduce experienta
void MainWindow::insertQuery(const QString &exp, const QString &experience_id)
{
    QSqlField idField("experience_id", QVariant::Int);
    QSqlField expField("exp", QVariant::String);
    idField.setValue(experience_id);
    expField.setValue(exp);
    QSqlRecord record;
    record.append(idField);
    record.append(expField);
    m_experienceModel->insertRecord(-1, record);
    if(!m_experienceModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

// adauga categorie varsta
void MainWindow::insertQuery1(const QString &minAge, const QString &maxAge, const QString &age_id)
{
    QSqlField idField("age_id", QVariant::Int);
    QSqlField minAgeField("min_bound", QVariant::Int);
    QSqlField maxAgeField("max_bound", QVariant::Int);
    idField.setValue(age_id);
    minAgeField.setValue(minAge);
    maxAgeField.setValue(maxAge);
    QSqlRecord record;
    record.append(idField);
    record.append(minAgeField);
    record.append(maxAgeField);
    m_ageModel->insertRecord(-1, record);
    if(!m_ageModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));

    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

// adacuga categorie greutate
void MainWindow::insertQuery2(const QString &min_bound, const QString &max_bound, const QString &weight_id)
{
    QSqlField idField("weight_id", QVariant::Int);
    QSqlField minWeightField("min_bound", QVariant::Int);
    QSqlField maxWeightField("max_bound", QVariant::Int);
    idField.setValue(weight_id);
    minWeightField.setValue(min_bound);
    maxWeightField.setValue(max_bound);
    QSqlRecord record;
    record.append(idField);
    record.append(minWeightField);
    record.append(maxWeightField);
    m_weightModel->insertRecord(-1, record);
    if(!m_weightModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

// adauga campionat
void MainWindow::insertQuery(const QString &name, const QString &location, const QString &championship_id,
                             const QString &qualification_score, const QString &quater_finals_score,
                             const QString &semifinals_score, const QString &finale_score)
{
    QSqlField idField("championship_id", QVariant::Int);
    QSqlField nameField("name", QVariant::String);
    QSqlField locationField("location", QVariant::String);
    QSqlField qualificationScoreField("qualification_score", QVariant::Int);
    QSqlField quaterfinalsScoreField("quater_finals_score", QVariant::Int);
    QSqlField semifinalsScoreField("semifinals_score", QVariant::Int);
    QSqlField finaleScoreField("finale_score", QVariant::Int);
    idField.setValue(championship_id);
    nameField.setValue(name);
    locationField.setValue(location);
    qualificationScoreField.setValue(qualification_score);
    quaterfinalsScoreField.setValue(quater_finals_score);
    semifinalsScoreField.setValue(semifinals_score);
    finaleScoreField.setValue(finale_score);
    QSqlRecord record;
    record.append(idField);
    record.append(nameField);
    record.append(locationField);
    record.append(qualificationScoreField);
    record.append(quaterfinalsScoreField);
    record.append(semifinalsScoreField);
    record.append(finaleScoreField);
    m_championshipModel->insertRecord(-1, record);
    if(!m_championshipModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
        qDebug()<<"Eroare champ: " << m_championshipModel->lastError();
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}


void MainWindow::fight(Participant participanti[],int players, int score, int stage)
{
    srand (time(NULL));
    int r, winnerId,loserId,j=0;
    //Championship champ;
    QString update1, update2;

    for(int i= 1; i<=players;i+=2)
    {
        if (stage==1) {
        r =rand() % 2;
        if(r)
        {
            qDebug() << participanti[i].getFirstName() << " a castigat";
            winnerId = participanti[i].getChampionshipId();
            loserId = participanti[i+1].getChampionshipId();
            qfParticipanti[++j].setFirstName(participanti[i].getFirstName());
            qfParticipanti[j].setLastName(participanti[i].getLastName());
            qfParticipanti[j].setChampionshipId(participanti[i].getChampionshipId());
        }
        else
        {
            qDebug() << participanti[i+1].getFirstName() << " a castigat";
            winnerId = participanti[i+1].getChampionshipId();
            loserId = participanti[i].getChampionshipId();
            qfParticipanti[++j].setFirstName(participanti[i+1].getFirstName());
            qfParticipanti[j].setLastName(participanti[i+1].getLastName());
            qfParticipanti[j].setChampionshipId(participanti[i+1].getChampionshipId());
        }
        }

        if (stage==2) {
        r =rand() % 2;
        if(r)
        {
            qDebug() << participanti[i].getFirstName() << " a castigat";
            winnerId = participanti[i].getChampionshipId();
            loserId = participanti[i+1].getChampionshipId();
            sfParticipanti[++j].setFirstName(participanti[i].getFirstName());
            sfParticipanti[j].setLastName(participanti[i].getLastName());
            sfParticipanti[j].setChampionshipId(participanti[i].getChampionshipId());
        }
        else
        {
            qDebug() << participanti[i+1].getFirstName() << " a castigat";
            winnerId = participanti[i+1].getChampionshipId();
            loserId = participanti[i].getChampionshipId();
            sfParticipanti[++j].setFirstName(participanti[i+1].getFirstName());
            sfParticipanti[j].setLastName(participanti[i+1].getLastName());
            sfParticipanti[j].setChampionshipId(participanti[i+1].getChampionshipId());
        }
        }

        if (stage==3) {
        r =rand() % 2;
        if(r)
        {
            qDebug() << participanti[i].getFirstName() << " a castigat";
            winnerId = participanti[i].getChampionshipId();
            loserId = participanti[i+1].getChampionshipId();
            fParticipanti[++j].setFirstName(participanti[i].getFirstName());
            fParticipanti[j].setLastName(participanti[i].getLastName());
            fParticipanti[j].setChampionshipId(participanti[i].getChampionshipId());
        }
        else
        {
            qDebug() << participanti[i+1].getFirstName() << " a castigat";
            winnerId = participanti[i+1].getChampionshipId();
            loserId = participanti[i].getChampionshipId();
            fParticipanti[++j].setFirstName(participanti[i+1].getFirstName());
            fParticipanti[j].setLastName(participanti[i+1].getLastName());
            fParticipanti[j].setChampionshipId(participanti[i+1].getChampionshipId());
        }
        }

        if (stage==4) {
        r =rand() % 2;
        if(r)
        {
            qDebug() << participanti[i].getFirstName() << " a castigat";
            winnerId = participanti[i].getChampionshipId();
            loserId = participanti[i+1].getChampionshipId();
        }
        else
        {
            qDebug() << participanti[i+1].getFirstName() << " a castigat";
            winnerId = participanti[i+1].getChampionshipId();
            loserId = participanti[1].getChampionshipId();
        }
        }


        if (stage ==1 ) {
        QSqlQuery query(m_db);
        update1 =QString("UPDATE championship SET qualification_score = '%1' WHERE championship_id = %2 ;").arg(score).arg(winnerId);
        update2 =QString("UPDATE championship SET qualification_score = '%1' WHERE championship_id = %2 ;").arg(1).arg(loserId);
        query.prepare(update1);
        query.exec();
        query.prepare(update2);
        query.exec();
        }
        if (stage ==2 ) {
        QSqlQuery query(m_db);
        update1 =QString("UPDATE championship SET quater_finals_score = '%1' WHERE championship_id = %2 ;").arg(score).arg(winnerId);
        update2 =QString("UPDATE championship SET quater_finals_score = '%1' WHERE championship_id = %2 ;").arg(11).arg(loserId);
        query.prepare(update1);
        query.exec();
        query.prepare(update2);
        query.exec();
        }
        if (stage ==3 ) {
        QSqlQuery query(m_db);
        update1 =QString("UPDATE championship SET semifinals_score = '%1' WHERE championship_id = %2 ;").arg(score).arg(winnerId);
        update2 =QString("UPDATE championship SET semifinals_score = '%1' WHERE championship_id = %2 ;").arg(21).arg(loserId);
        query.prepare(update1);
        query.exec();
        query.prepare(update2);
        query.exec();
        }
        if (stage ==4  ) {
        QSqlQuery query(m_db);
        update1 =QString("UPDATE championship SET finale_score = '%1' WHERE championship_id = %2 ;").arg(score).arg(winnerId);
        update2 =QString("UPDATE championship SET finale_score = '%1' WHERE championship_id = %2 ;").arg(31).arg(loserId);
        query.prepare(update1);
        query.exec();
        query.prepare(update2);
        query.exec();
        }

    }
}
void MainWindow::clearScore()
{
    QString update1;
    for(int i=1;i<=16;i++) {
    QSqlQuery query(m_db);
    update1 =QString("UPDATE championship SET  finale_score = '%1', semifinals_score = '%2', quater_finals_score = '%3',qualification_score = '%4' WHERE championship_id = %5 ;").arg(0).arg(0).arg(0).arg(0).arg(i);
    query.prepare(update1);
    query.exec();
    }
}

void MainWindow::simulateFights()
{
    //clearScore();
    qDebug()<<"Qualification: ";
    fight(participanti,16,10,1);
    qDebug()<<"QuaterFinals: ";
    fight(qfParticipanti,8,20,2);
    qDebug()<<"Semifinals: ";
    fight(sfParticipanti,4,30,3);
    qDebug()<<"Finals: ";
    fight(fParticipanti,2,40,4);

}

Championship& MainWindow::getChampionshipById(int id)
{
    for (int i=1; i<= 100; i++)
    if(championship[i].getId() == id)
        return championship[i];
}

void MainWindow::iterateParticipants()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Participanti");
    query.exec();
    int i=0;
    while(query.next()) {


        Championship champ;
        //int id = query.value("participant_id").toInt();
        QString firstName = query.value("first_name").toString();
        QString lastName = query.value("last_name").toString();
        int championshipId = query.value("championship_id").toInt();
        participanti[++i].setFirstName(firstName);
        participanti[i].setLastName(lastName);
        participanti[i].setChampionshipId(championshipId);
        //int cid = participanti[i].getChampionshipId();
       // champ = getChampionshipById(cid);

//        qDebug()<<"First Name: "  <<participanti[i].getFirstName();
//        qDebug()<<"Last Name: "  <<participanti[i].getLastName();
//        qDebug()<<"Cahmpionship id: "  <<cid;
//        qDebug()<< "Nume Campionat: "  <<champ.getName();
    }

}

void MainWindow::iterateChampionship()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM championship");
    query.exec();
    int i=0;
    while(query.next()) {

        int id = query.value("championship_id").toInt();
        QString name = query.value("name").toString();
        QString location = query.value("location").toString();
        championship[++i].setId(id);
        //qDebug()<<"Cahmpionship id: "  <<id;
        championship[i].setName(name);
        championship[i].setLocation(location);


    }

}

int MainWindow::calculateWeightId(int weight){
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM weight");
    query.exec();
    while(query.next()) {

        int id = query.value("weight_id").toInt();
        int min_bound = query.value("min_bound").toInt();
        int max_bound = query.value("max_bound").toInt();
        if(weight <= max_bound && weight > min_bound ){
            return id;
        }
    }
    return 0;
}

int MainWindow::calculateAgeId(int age){
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM age");
    query.exec();
    while(query.next()) {

        int id = query.value("age_id").toInt();
        int min_bound = query.value("min_bound").toInt();
        int max_bound = query.value("max_bound").toInt();
        if(age <= max_bound && age > min_bound ){
            return id;
        }
    }
    return 0;
}


void MainWindow::selectQuery()
{
    m_personsModel->select();

}

void MainWindow::onNextRound()
{
         QSqlQuery query("SELECT Participanti.first_name, Participanti.last_name, championship.qualification_score, championship.quater_finals_score, championship.semifinals_score, championship.finale_score FROM Participanti INNER JOIN championship ON Participanti.championship_id=championship.championship_id ORDER BY championship.finale_score DESC, championship.semifinals_score DESC, championship.quater_finals_score DESC, championship.qualification_score DESC");
         m_participantJoinChampModel->setQuery(query);
         //simulateFights();
         QTableView *view = new QTableView;
         view->setModel(m_participantJoinChampModel);
         view->setFixedWidth(620);
         view->setFixedHeight(600);
         view->show();
}

void MainWindow::onTableActionsTriggered(QAction *action)
{
    if(action == ui->actionTables)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(action == ui->actionOrganizations)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->tableOrganization->selectionModel()->clearSelection();
    }

    if(action == ui->actionChampionship)
    {
        ui->stackedWidget->setCurrentIndex(7);
        ui->tableChampionship->selectionModel()->clearSelection();

    }
    if(action == ui->actionAge)
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->tableAge->selectionModel()->clearSelection();
    }
    if(action == ui->actionWeight)
    {
        ui->stackedWidget->setCurrentIndex(3);
        ui->tableAge->selectionModel()->clearSelection();
    }
    if(action == ui->actionExperience)
    {
        ui->stackedWidget->setCurrentIndex(4);
        ui->tableExperience->selectionModel()->clearSelection();
    }
}


