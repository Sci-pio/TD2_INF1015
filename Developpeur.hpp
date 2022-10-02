// Auteurs: Leonard Pouliot (2150965) et Kamil Maarite (2152653)
// Date: 1 octobre 2022
// cours: INF1015
// Nom de la classe: Developpeur.hpp

#pragma once
#include "ListeJeux.hpp"
#include <string>
#include "Jeu.hpp"
#include "gsl/span"
#include <iostream>

using namespace std;
using namespace gsl;


class Developpeur
{	
public:
	Developpeur();
	Developpeur(const string& nom);
	const string& obtenirNom() const;
	int compterNombreJeux(const ListeJeux& listeJeux) const;
	void mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux);
	void afficherJeuxDeveloppes();
	~Developpeur();

private:
	pair<string, ListeJeux> paireNomJeux_;
};


Developpeur::Developpeur()
{
	paireNomJeux_.first = "Pas de nom";
	paireNomJeux_.second = {};
}


Developpeur::Developpeur(const string& nom)
{
	paireNomJeux_.first = nom;
	paireNomJeux_.second = {};
}


const string& Developpeur::obtenirNom() const
{
	return paireNomJeux_.first;
}


int Developpeur::compterNombreJeux(const ListeJeux& listeJeux) const
{
	int nbJeux = 0;
	for (Jeu* ptrJeu : span(listeJeux.elements, listeJeux.nElements)) {
		if (ptrJeu->developpeur == obtenirNom())
			nbJeux++;
	}
	return nbJeux;
}


void Developpeur::mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux)
{
	int tailleListeJeux = compterNombreJeux(listeTousLesJeux);
	int index = 0;

	delete[] paireNomJeux_.second.elements;
	paireNomJeux_.second.elements = new Jeu*[tailleListeJeux];
	paireNomJeux_.second.capacite = tailleListeJeux;

	for (Jeu* ptrJeu : span(listeTousLesJeux.elements, listeTousLesJeux.nElements))
		if (ptrJeu->developpeur == obtenirNom()) {
			paireNomJeux_.second.elements[index] = ptrJeu; // Warning pcq le compilateur a peur que l'index depasse la taille du tableau, dans notre cas ca devrait etre correct à cause de compterNbJeux()
			paireNomJeux_.second.nElements++;
			index++;
		}
}


void Developpeur::afficherJeuxDeveloppes()
{
	cout << "Les jeux développés par " << paireNomJeux_.first << " sont: " << endl;

	for (Jeu* ptrJeu : span(paireNomJeux_.second.elements, paireNomJeux_.second.nElements))
		cout << ptrJeu->titre << endl;
}


Developpeur::~Developpeur()
{
	delete[] paireNomJeux_.second.elements;
}