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
	void ajouterDeveloppeur(Developpeur& developpeur);
	void retirerDeveloppeur(Developpeur& developpeur);

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

}

void ListeDeveloppeurs:: ajouterDeveloppeur(Developpeur& developpeur)
{

}

void ListeDeveloppeurs::retirerDeveloppeur(Developpeur& developpeur)
{

}

// Destructeur
ListeDeveloppeurs:: ~ListeDeveloppeurs()
{
	delete[] elements_;
}