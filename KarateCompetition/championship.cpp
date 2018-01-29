#include "championship.h"

Championship::Championship()
{

}



void Championship::setId(int id)
{
    this->id=id;
}
void Championship::setName(QString name)
{
    this->name=name;
}
void Championship::setLocation(QString location)
{
    this->location=location;
}
void Championship::setQualificationScore(int score)
{
    this->qualificationScore=score;
}
void Championship::setQuaterfinalsScore(int score)
{
    this->quaterfinalsScore=score;
}
void Championship::setSemifinalsScore(int score)
{
    this->semifinalsScore=score;
}
void Championship::setFinalScore(int score)
{
    this->finalScore=score;
}
int Championship::getId()
{
    return this->id;
}
QString Championship::getName()
{
    return this->name;
}
QString Championship::getLocation()
{
    return this->location;
}
int Championship::getQualificationScore()
{
    return this->qualificationScore;
}
int Championship::getQuaterfinalsScore()
{
    return this->quaterfinalsScore;
}
int Championship::getSemifinalsScore()
{
    return this->semifinalsScore;
}
int Championship::getFinalScore()
{
    return this->finalScore;
}
