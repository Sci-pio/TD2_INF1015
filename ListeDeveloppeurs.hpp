// Auteurs: Leonard Pouliot (2150965) et Kamil Maarite (2152653)
// Date: 1 octobre 2022
// cours: INF1015
// Nom de la classe: ListeDeveloppeur.hpp

#pragma once
#include <cstddef>
#include "Developpeur.hpp"
#include "gsl/span"

using namespace gsl;
using namespace std;

class ListeDeveloppeurs
{
public:
    ListeDeveloppeurs();
    void afficher() const;
    void ajouterDeveloppeur(Developpeur* ptrDeveloppeur);
    void retirerDeveloppeur(Developpeur* ptrDeveloppeur);
    ~ListeDeveloppeurs();

private:
    size_t nElements_, capacite_;
    Developpeur** elements_;
};

ListeDeveloppeurs::ListeDeveloppeurs()
{
    elements_ = {};
    nElements_ = 0;
    capacite_ = 0;
}

void ListeDeveloppeurs::afficher() const
{
    cout << "--------------------------------------------------" << endl;
    cout << "La ListeDeveloppeurs possède " << nElements_ << " développeur(s)" << endl;
    for (Developpeur* developpeur : span(elements_, nElements_))
        developpeur->afficherJeuxDeveloppes();
}

void ListeDeveloppeurs::ajouterDeveloppeur(Developpeur* ptrDeveloppeur)
{
    bool estListeVide = capacite_ == 0;
    bool estListePleine = capacite_ - 1 <= nElements_;

    if (estListePleine || estListeVide)
    {
        size_t nouvelleCapacite = estListeVide ? capacite_ = 1 : capacite_ * 2;
        Developpeur** nouvelleListeJeux = new Developpeur * [nouvelleCapacite];

        for (size_t i = 0; i < nElements_; i++)
            nouvelleListeJeux[i] = elements_[i]; // Warning pcq le compilateur a peur que l'on depace la taile de la liste, dans notre cas c'est impossible que ca arrive.

        delete[] elements_;

        elements_ = nouvelleListeJeux;
        capacite_ = nouvelleCapacite;
    }
    elements_[nElements_] = ptrDeveloppeur; // Warning pcq le compilateur a peur que l'on depace la taile de la liste, dans notre cas c'est impossible que ca arrive.
    nElements_++;
}

void ListeDeveloppeurs::retirerDeveloppeur(Developpeur* ptrDeveloppeur)
{
    int indexDeveloppeur = -1;

    for (int i = 0; i < nElements_; i++)
    {
        if (elements_[i] == ptrDeveloppeur)
            indexDeveloppeur = i;
    }

    bool estJeuPresent = indexDeveloppeur != -1;
    bool estListeVide = nElements_ <= 0;

    if (estJeuPresent && !estListeVide)
    {
        Developpeur* ptrDernierDeveloppeur = elements_[nElements_ - 1];
        elements_[indexDeveloppeur] = ptrDernierDeveloppeur;
        elements_[nElements_ - 1] = nullptr;
        nElements_--;
    }
}

ListeDeveloppeurs::~ListeDeveloppeurs()
{
    delete[] elements_;
}