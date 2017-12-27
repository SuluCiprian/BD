/**
**
**  TODO: conexiunea pentru butonul save
**
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mParticipanti()
{
    ui->setupUi(this);
    connect(ui->addParticipant, &QPushButton::clicked, this, &MainWindow::addParticipant);
}

void MainWindow::addParticipant()
{
    qDebug() << "Adding new Participant";

    Participant* participant = new Participant();

    //Adauga participantul in vector
    mParticipanti.append(participant);

    //Afiseaza numele in consola
    qDebug() << participant->name();

    //Adauga in fereastra principala
    ui->verticalLayout->addWidget(participant);
}

MainWindow::~MainWindow()
{
    delete ui;
}
