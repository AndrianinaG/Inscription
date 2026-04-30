#include"Etudiant.h"

Etudiant::Etudiant()
{

}

Etudiant::~Etudiant()
{

}

QString Etudiant::getPrenom()
{
    return e_prenom;
}

int Etudiant::getAge()
{
    return e_age;
}

void Etudiant::setPrenom(QString prenom)
{
    e_prenom = prenom;
}

void Etudiant::setAge(int age)
{
    e_age = age;
}