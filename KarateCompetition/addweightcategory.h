#ifndef ADDWEIGHTCATEGORY_H
#define ADDWEIGHTCATEGORY_H


#include <QDialog>

namespace Ui {
class AddWeightCategory;
}

class AddWeightCategory : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddWeightCategory(QWidget *parent = 0);
    ~AddWeightCategory();

    void setData(const QString &min_bound, const QString &max_bound, const QString &weight_id);
    void data( QString &min_bound,  QString &max_bound,  QString &weight_id);
    void setType(AddType type);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddWeightCategory *ui;
    AddType m_type;
};

#endif // ADDWEIGHTCATEGORY_H
