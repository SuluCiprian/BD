#ifndef AddParticipant_H
#define AddParticipant_H

#include <QDialog>

namespace Ui {
class AddParticipant;
}

class AddParticipant : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddParticipant(QWidget *parent = 0);
    ~AddParticipant();

    void setData(const QString &firstName, const QString &lastName, const QString &id,
                 const QString &age, const QString &weight, const QString &experience, const QString &organization_id, const QString &championship_id);
    void data(QString &firstName, QString &lastName, QString &id,
               QString &age,  QString &weight,  QString &experience,  QString &organization_id,  QString &championship_id);
   // void data(QString &name, QString &info, QString &id);
    void setType(AddType type);
    //void setHours(const QString &hours);
   // void hours(QString &hours);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddParticipant *ui;
    AddType m_type;
};

#endif // AddParticipant_H
