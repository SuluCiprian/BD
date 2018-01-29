#include "addexperiencecategory.h"
#include "ui_addexperiencecategory.h"
#include <QMessageBox>

AddExperienceCategory::AddExperienceCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddExperienceCategory),
    m_type(AddExperienceCategory::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddExperienceCategory::~AddExperienceCategory()
{
    delete ui;
}

void AddExperienceCategory::setData(const QString &exp, const QString &experience_id)
{
    ui->exp->setText(exp);
    ui->experience_id->setText(experience_id);
}
void AddExperienceCategory::data( QString &exp,  QString &experience_id)
{
    exp = ui->exp->text();
    experience_id = ui->experience_id->text();
}

void AddExperienceCategory::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->experience_id->text().toInt(&ok);
        if(!ok)
        {
            QMessageBox::warning(this, tr("Add Item Error"), tr("Id should be a number"));
        }
        else
        {
            QDialog::accept();
        }
    }
}

void AddExperienceCategory::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "");
        ui->experience_id->setEnabled(true);
        ui->exp->setEnabled(true);
    }
}
void AddExperienceCategory::setType(AddType type)
{
    m_type = type;
}
