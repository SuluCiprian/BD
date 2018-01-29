#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include <QString>
class Participant
{
private:
    int id;
    QString firstName;
    QString lastName;
    int age;
    int weight;
    QString experience;
    int championshipId;
    int organizationId;
    int weightId;
    int ageId;
    int experienceId;
public:
    Participant(int id, QString firstName, QString lastName,int age,int weight, QString experience,int championshipId,int organizationId,int weightId,int ageId,int experienceId);
    Participant();
    //~Participant();
    void setId(int id);
    void setFirstName(QString name);
    void setLastName(QString name);
    void setAge(int age);
    void setWeight(int weight);
    void setExperiemce(QString exp);
    void setChampionshipId(int id);
    void setOrganizationId(int id);
    void setWeightId(int id);
    void setAgeId(int id);
    void setExperiemceId(int id);

    int getId();
    QString getFirstName();
    QString getLastName();
    int getAge();
    int getWeight();
    QString getExperiemce();
    int getChampionshipId();
    int getOrganizationId();
    int getWeightId();
    int getAgeId();
    int getExperiemceId();
};

#endif // PARTICIPANT_H
