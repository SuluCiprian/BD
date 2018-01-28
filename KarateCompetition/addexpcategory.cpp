#include "addexpcategory.h"
#include "ui_addexpcategory.h"
#include <QMessageBox>

AddExpCategory::AddExpCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddExpCategory),
    m_type(AddExpCategory::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddExpCategory::~AddExpCategory()
{
    delete ui;
}

void AddExpCategory::setData(const QString &experience_id, const QString &exp)
{
    ui->experience_id->setText(experience_id);
    ui->exp->setText(exp);
}
void AddExpCategory::data(QString &experience_id, QString &exp)
{
    experience_id = ui->experience_id->text();
    exp = ui->exp->text();
}

void AddExpCategory::accept()
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

void AddExpCategory::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "");
        ui->experience_id->setEnabled(true);
        ui->exp->setEnabled(true);
    }
}
void AddExpCategory::setType(AddType type)
{
    m_type = type;
}
