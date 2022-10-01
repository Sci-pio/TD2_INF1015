#pragma once
#include <cstddef>
#include "Developpeur.hpp"
#include "gsl/span"

class ListeDeveloppeurs
{
	//TODO: En faire une classe qui suit les principes OO.
	//TODO: On veut pouvoir ajouter et enlever un Developpeur* de la liste, avec réallocation dynamique tel que faite pour ListeJeux.
	//NOTE: Le code sera principalement copié de certaines fonctions écrites pour la partie 1, mais mises dans une classe.
public:
	ListeDeveloppeurs();
	void afficher() const;
	void ajouterDeveloppeur(Developpeur* ptrDeveloppeur);
	void retirerDeveloppeur(Developpeur* ptrDeveloppeur);

private:
	std::size_t nElements_, capacite_;
	Developpeur** elements_;
};


//Constructeur par defaut
ListeDeveloppeurs::ListeDeveloppeurs() 
{
	elements_ = {};
	nElements_ = 0; capacite_ = 0;
}


void ListeDeveloppeurs::afficher() const
{
	for (Developpeur* developpeur : span(elements_, nElements_))
		developpeur->afficherJeuxDeveloppes();
}


void ListeDeveloppeurs:: ajouterDeveloppeur(Developpeur* ptrDeveloppeur)
{
	if (capacite_ - 1 <= nElements_ || capacite_ == 0)
	{
		size_t nouvelleCapacite = capacite_ == 0 ? capacite_ = 1 : capacite_ * 2;
		Developpeur** nouvelleListeJeux = new Developpeur * [nouvelleCapacite];

		for (size_t i = 0; i < nElements_; i++)
			nouvelleListeJeux[i] = elements_[i];

		delete[] elements_;

		elements_ = nouvelleListeJeux;
		capacite_ = nouvelleCapacite;
	}

	elements_[nElements_] = ptrDeveloppeur;
	nElements_++;

	
}


void ListeDeveloppeurs::retirerDeveloppeur(Developpeur* ptrDeveloppeur)
{
	int indexDeveloppeur = -1;

	for (size_t i = 0; i < nElements_; i++) {
		if (elements_[i] == ptrDeveloppeur)
			indexDeveloppeur = i;
	}

	if (indexDeveloppeur != -1 && nElements_ >= 0) {
		Developpeur* ptrDernierDeveloppeur = elements_[nElements_ - 1];
		elements_[indexDeveloppeur] = ptrDernierDeveloppeur;
		elements_[nElements_ - 1] = nullptr;
		nElements_--;
	}
}


// Destructeur
ListeDeveloppeurs:: ~ListeDeveloppeurs()
{
	delete[] elements_;
}