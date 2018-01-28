#ifndef ADDEXPCATEGORY_H
#define ADDEXPCATEGORY_H

#include <QDialog>

namespace Ui {
class AddExpCategory;
}

class AddExpCategory : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddExpCategory(QWidget *parent = 0);
    ~AddExpCategory();

    void setData(const QString &experience_id, const QString &exp);
    void data(QString &experience_id, QString &exp);
    void setType(AddType type);
    //void setHours(const QString &hours);
   // void hours(QString &hours);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddExpCategory *ui;
    AddType m_type;
};

#endif // AddExpCategory_H
