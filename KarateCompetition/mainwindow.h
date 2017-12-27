#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "participant.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addParticipant();

private:
    Ui::MainWindow *ui;
    QVector<Participant*> mParticipanti;
};

#endif // MAINWINDOW_H
