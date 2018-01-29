#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H
#include <QString>

class Championship
{
private:
    int id;
    QString name;
    QString location;
    int qualificationScore;
    int quaterfinalsScore;
    int semifinalsScore;
    int finalScore;
public:
    //Championship(int id, QString name, QString location,int qualificationScore,int quaterfinalsScore, int semifinalsScore,int finalScore);
    Championship();
    //~Participant();
    void setId(int id);
    void setName(QString name);
    void setLocation(QString location);
    void setQualificationScore(int score);
    void setQuaterfinalsScore(int score);
    void setSemifinalsScore(int score);
    void setFinalScore(int score);
    int getId();
    QString getName();
    QString getLocation();
    int getQualificationScore();
    int getQuaterfinalsScore();
    int getSemifinalsScore();
    int getFinalScore();


};

#endif // CHAMPIONSHIP_H
