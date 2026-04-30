#ifndef INSCRIPTION_H
#define INSCRIPTION_H
#include"Etudiant.h"
#include<vector>

using namespace std;
class Inscription
{
    public:
        vector<Etudiant> liste;

        Inscription();
        ~Inscription();
        void ajouter(Etudiant student);
        vector<Etudiant> getList();
        int getSize();
};

#endif // INSCRIPTION_H
