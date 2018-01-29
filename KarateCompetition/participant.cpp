#include "participant.h"

Participant::Participant(int id, QString firstName, QString lastName,int age,int weight, QString experience,int championshipId,int organizationId,int weightId,int ageId,int experienceId)
{
    this->id=id;
    this->firstName=firstName;
    this->lastName=lastName;
    this->age=age;
    this->weight=weight;
    this->experience=experience;
    this->championshipId=championshipId;
    this->organizationId=organizationId;
    this->weightId=weightId;
    this->ageId=ageId;
    this->experienceId=experienceId;
}
Participant::Participant()
{

}
void Participant::setId(int id)
{
    this->id=id;
}

void Participant::setFirstName(QString name)
{
    this->firstName=name;
}
void Participant::setLastName(QString name)
{
    this->lastName=name;
}
void Participant::setAge(int age)
{
    this->age=age;
}
void Participant::setWeight(int weight)
{
    this->weight=weight;
}
void Participant::setExperiemce(QString exp)
{
    this->experience=exp;
}
void Participant::setChampionshipId(int id)
{
    this->championshipId=id;
}
void Participant::setOrganizationId(int id)
{
    this->organizationId=id;
}
void Participant::setWeightId(int id)
{
    this->weightId=id;
}
void Participant::setAgeId(int id)
{
    this->ageId=id;
}
void Participant::setExperiemceId(int id)
{
    this->experienceId=id;
}

int Participant::getId()
{
    return this->id;
}
QString Participant::getFirstName()
{
    return this->firstName;
}
QString Participant::getLastName()
{
    return this->lastName;
}
int Participant::getAge()
{
    return this->age;
}
int Participant::getWeight()
{
    return this->weight;
}
QString Participant::getExperiemce()
{
    return this->experience;
}
int Participant::getChampionshipId()
{
    return this->championshipId;
}
int Participant::getOrganizationId()
{
    return this->organizationId;
}
int Participant::getWeightId()
{
    return this->weightId;
}
int Participant::getAgeId()
{
    return this->ageId;
}
int Participant::getExperiemceId()
{
    return this->experienceId;
}
