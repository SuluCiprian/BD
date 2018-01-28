#include "addparticipant.h"
#include "ui_AddParticipant.h"

#include <QMessageBox>

AddParticipant::AddParticipant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddParticipant),
    m_type(AddParticipant::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddParticipant::~AddParticipant()
{
    delete ui;
}

void AddParticipant::setData(const QString &firstName, const QString &lastName, const QString &id,
                            const QString &age, const QString &weight, const QString &experience)
{
    ui->firstName->setText(firstName);
    ui->lastName->setText(lastName);
    ui->id->setText(id);
    ui->age->setText(age);
    ui->weight->setText(weight);
    ui->experience->setText(experience);
}

//void AddParticipant::data(QString &name, QString &info, QString &id)
//{
//    name = ui->name->text();
//    info = ui->info->text();
//    id = ui->id->text();

//}
void AddParticipant::data(QString &firstName, QString &lastName, QString &id,
                          QString &age,  QString &weight,  QString &experience)
{
    firstName = ui->firstName->text();
    lastName = ui->lastName->text();
    id = ui->id->text();
    age = ui->age->text();
    weight = ui->weight->text();
    experience = ui->experience->text();
}

void AddParticipant::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->id->text().toInt(&ok);
        if(!ok)
        {
            QMessageBox::warning(this, tr("Add Item Error"), tr("Id should be a number"));
        }
        else
        {
            QDialog::accept();
        }
    }

//    if(m_type == AddType::ADD_HOURS)
//    {
//        ui->hours->text().toInt(&ok);
//        if(!ok)
//        {
//            QMessageBox::warning(this, tr("Add Hours Error"), tr("Hours should be a number"));
//        }
//        else
//        {
//            QDialog::accept();
//        }
//    }
}

void AddParticipant::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "", "", "", "", "");
        ui->firstName->setEnabled(true);
        ui->lastName->setEnabled(true);
        ui->id->setEnabled(true);
        ui->age->setEnabled(true);
        ui->weight->setEnabled(true);
        ui->experience->setEnabled(true);

//        ui->hours->setVisible(false);
//        ui->labelHours->setVisible(false);
    }

    if(m_type == AddType::ADD_HOURS)
    {
       // setHours("");
        ui->firstName->setEnabled(false);
        ui->lastName->setEnabled(false);
        ui->id->setEnabled(false);
        ui->age->setEnabled(false);
        ui->weight->setEnabled(false);
        ui->experience->setEnabled(false);
//        ui->hours->setVisible(true);
//        ui->labelHours->setVisible(true);
    }
}

void AddParticipant::setType(AddType type)
{
    m_type = type;
}

//void AddParticipant::setHours(const QString &hours)
//{
//    ui->hours->setText(hours);
//}

//void AddParticipant::hours(QString &hours)
//{
//    hours = ui->hours->text();
//}
