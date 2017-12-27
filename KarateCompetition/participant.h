#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QWidget>

namespace Ui {
class Participant;
}

class Participant : public QWidget
{
    Q_OBJECT

public:
    explicit Participant(QWidget *parent = 0);
    ~Participant();
    QString name() const;

private:
    Ui::Participant *ui;
};

#endif // PARTICIPANT_H
