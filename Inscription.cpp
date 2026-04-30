#include"Inscription.h"

Inscription::Inscription()
{

}
Inscription::~Inscription()
{

}

void Inscription::ajouter(Etudiant student)
{
    liste.push_back(student);
}

vector<Etudiant> Inscription::getList()
{
    return liste;
}

int Inscription::getSize()
{
    return liste.size();
}
