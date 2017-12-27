#include "participant.h"
#include "ui_participant.h"
#include <QDebug>
Participant::Participant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Participant)
{
    ui->setupUi(this);
}

QString Participant::name() const
{
    //Returneaza numele
    return ui->nameInput->text();

}

Participant::~Participant()
{
    delete ui;
}
