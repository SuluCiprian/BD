#include "addweightcategory.h"
#include "ui_addweightcategory.h"

#include <QMessageBox>

AddWeightCategory::AddWeightCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWeightCategory),
    m_type(AddWeightCategory::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddWeightCategory::~AddWeightCategory()
{
    delete ui;
}

void AddWeightCategory::setData(const QString &min_bound, const QString &max_bound, const QString &weight_id)
{
    ui->min_bound->setText(min_bound);
    ui->max_bound->setText(max_bound);
    ui->weight_id->setText(weight_id);
}
void AddWeightCategory::data(QString &min_bound, QString &max_bound, QString &weight_id)
{
    min_bound = ui->min_bound->text();
    max_bound = ui->max_bound->text();
    weight_id = ui->weight_id->text();
}

void AddWeightCategory::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->weight_id->text().toInt(&ok);
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

void AddWeightCategory::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "", "");
        ui->weight_id->setEnabled(true);
        ui->min_bound->setEnabled(true);
        ui->max_bound->setEnabled(true);
    }
}
void AddWeightCategory::setType(AddType type)
{
    m_type = type;
}
