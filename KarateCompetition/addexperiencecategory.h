#ifndef ADDEXPERIENCECATEGORY_H
#define ADDEXPERIENCECATEGORY_H


#include <QDialog>

namespace Ui {
class AddExperienceCategory;
}

class AddExperienceCategory : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddExperienceCategory(QWidget *parent = 0);
    ~AddExperienceCategory();

    void setData(const QString &exp, const QString &experience_id);
    void data( QString &exp,  QString &experience_id);
    void setType(AddType type);

protected:
    void accept() override;

private:
    Ui::AddExperienceCategory *ui;
    AddType m_type;
};

#endif // ADDEXPERIENCECATEGORY_H
