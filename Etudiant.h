#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <QString>

class Etudiant
{
private:
    QString e_prenom;
    int e_age;

public:
    Etudiant();
    ~Etudiant();

    void setPrenom(QString prenom);
    void setAge(int age);

    QString getPrenom();
    int getAge();
};

#endif // ETUDIANT_H
