#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemdialog.h"
#include "addorganization.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSettings>
#include <QActionGroup>
#include <QItemSelectionModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tableActions = new QActionGroup(this);
    m_tableActions->addAction(ui->actionTables);
    m_tableActions->addAction(ui->actionWorking_Hours);
    m_tableActions->addAction(ui->actionJoin_Table);
    connect(m_tableActions, &QActionGroup::triggered, this, &MainWindow::onTableActionsTriggered);
    connect(ui->actionAdd_Item, &QAction::triggered, this, &MainWindow::onAddItem);
    connect(ui->actionAdd_Organization, &QAction::triggered, this, &MainWindow::onAddOrganization);
    m_addItemDialog = new AddItemDialog(this);
    m_addOrganization = new AddOrganization(this);
    QString hostName = "horton.elephantsql.com";
    QString databaseName = "vefisijm";
    QString userName = "vefisijm";
    QString password = "9FBCK4VnPwUt2T8sXOkSigfd4XQTGZO-";
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
    }
    setupModel();
    ui->tablePersons->setModel(m_personsModel);\
    ui->tableOrganization->setModel(m_organizationModel);
    ui->tableWorkingHours->setModel(m_workingHoursModel);
    ui->tableWorkingHoursJoinPersons->setModel(m_workingHoursJoinPersonsModel);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::onRefreshDB);
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
    m_addItemDialog->setType(AddItemDialog::AddType::ADD_PERSON);
    int r = m_addItemDialog->exec();
    if(r == QDialog::Accepted)
    {
        QString firstName;
        QString lastName;
        QString id;
        QString age;
        QString weight;
        QString experience;
        m_addItemDialog->data(firstName, lastName, id, age, weight, experience);
        insertQuery(id, firstName, lastName, age, weight, experience);
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

void MainWindow::onRefreshDB()
{
    selectQuery();
}

void MainWindow::setupModel()
{
    ui->tablePersons->setSelectionMode(QAbstractItemView::SingleSelection);
    m_personsModel = new QSqlTableModel(this, m_db);
    m_personsModel->setTable("Participanti");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_personsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_personsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_personsModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m_personsModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m_personsModel->select();

    ui->tableOrganization->setSelectionMode(QAbstractItemView::SingleSelection);
    m_organizationModel = new QSqlTableModel(this, m_db);
    m_organizationModel->setTable("organization");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_organizationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_organizationModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_organizationModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_organizationModel->setHeaderData(2, Qt::Horizontal, tr("Info"));
    m_organizationModel->select();

    m_workingHoursModel = new QSqlRelationalTableModel(this, m_db);
    m_workingHoursModel->setTable("WorkingHours");
    m_workingHoursModel->setRelation(0, QSqlRelation("Persons", "id", "lastname"));
    m_workingHoursModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_workingHoursModel->setHeaderData(0, Qt::Horizontal, tr("Person"));
    m_workingHoursModel->setHeaderData(1, Qt::Horizontal, tr("Hours"));
    m_workingHoursModel->select();

    m_workingHoursJoinPersonsModel = new QSqlQueryModel(this);
    QSqlQuery query("SELECT workinghours.id, persons.firstname, persons.lastname, workinghours.hours \
                       FROM workinghours INNER JOIN persons ON workinghours.id=persons.id");
    m_workingHoursJoinPersonsModel->setQuery(query);
    m_workingHoursJoinPersonsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_workingHoursJoinPersonsModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m_workingHoursJoinPersonsModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m_workingHoursJoinPersonsModel->setHeaderData(3, Qt::Horizontal, tr("Hours"));
}

void MainWindow::insertQuery(const QString &id, const QString &firstName, const QString &lastName,
                             const QString &age, const QString &weight, const QString &experience)
{
    QSqlField idField("participant_id", QVariant::Int);
    QSqlField firstNameField("first_name", QVariant::String);
    QSqlField lastNameField("last_name", QVariant::String);
    QSqlField ageField("age", QVariant::Int);
    QSqlField weightField("weight", QVariant::Int);
    QSqlField experienceField("experience", QVariant::String);
    idField.setValue(id);
    firstNameField.setValue(firstName);
    lastNameField.setValue(lastName);
    ageField.setValue(age);
    weightField.setValue(weight);
    experienceField.setValue(experience);
    QSqlRecord record;
    record.append(idField);
    record.append(firstNameField);
    record.append(lastNameField);
    record.append(ageField);
    record.append(weightField);
    record.append(experienceField);
    m_personsModel->insertRecord(-1, record);
    if(!m_personsModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

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

void MainWindow::insertQuery(const QString &id, const QString &hours)
{
    QSqlField idField("id", QVariant::Int);
    QSqlField hoursField("hours", QVariant::Int);
    idField.setValue(id);
    hoursField.setValue(hours);
    QSqlRecord record;
    record.append(idField);
    record.append(hoursField);
    m_workingHoursModel->insertRecord(-1, record);
    if(!m_workingHoursModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::selectQuery()
{
    m_personsModel->select();
    m_workingHoursModel->select();
    QSqlQuery query("SELECT workinghours.id, persons.firstname, persons.lastname, workinghours.hours \
                       FROM workinghours INNER JOIN persons ON workinghours.id=persons.id");
    m_workingHoursJoinPersonsModel->setQuery(query);
}

void MainWindow::onTableActionsTriggered(QAction *action)
{
    if(action == ui->actionTables)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tablePersons);
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(action == ui->actionWorking_Hours)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tableWorkingHours);
        ui->stackedWidget->setCurrentIndex(1);
        ui->tablePersons->selectionModel()->clearSelection();
    }
    if(action == ui->actionJoin_Table)
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->tablePersons->selectionModel()->clearSelection();
    }
}

void MainWindow::onAddHours()
{
    QItemSelectionModel *selModel = ui->tablePersons->selectionModel();
    QModelIndexList selIndexes = selModel->selectedIndexes();
    if(selIndexes.count() == 0)
    {
        return;
    }
    QModelIndex index = selIndexes[0];
    int row = index.row();
    QString id = m_personsModel->itemData(index.sibling(row, 0))[Qt::EditRole].toString();
    QString firstName = m_personsModel->itemData(index.sibling(row, 1))[Qt::EditRole].toString();
    QString lastName = m_personsModel->itemData(index.sibling(row, 2))[Qt::EditRole].toString();
    QString age = m_personsModel->itemData(index.sibling(row, 3))[Qt::EditRole].toString();
    QString weight = m_personsModel->itemData(index.sibling(row, 4))[Qt::EditRole].toString();
    QString experience = m_personsModel->itemData(index.sibling(row, 5))[Qt::EditRole].toString();

    m_addItemDialog->setType(AddItemDialog::AddType::ADD_HOURS);
    m_addItemDialog->setData(firstName, lastName, id, age, weight, experience);
    int r = m_addItemDialog->exec();
    if(r == QDialog::Rejected)
    {
        return;
    }
//    QString hours;
//    m_addItemDialog->hours(hours);
//    insertQuery(id, hours);
}
