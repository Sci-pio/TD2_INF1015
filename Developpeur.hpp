#pragma once
#include "ListeJeux.hpp"
#include "gsl/span"
#include "Jeu.hpp"
#include <iostream>

using namespace std;
using namespace gsl;

class Developpeur
{
	//TODO: En faire une classe qui suit les principes OO.
	//TODO: La destruction d'un Developpeur doit s'assurer que la désallocation de ListeJeux est faite.
	//TODO: Les méthodes à faire...
	
public:
	Developpeur(const string& nom);
	const std::string& obtenirNom() const;
	int compterNbJeux(const ListeJeux& listeJeux) const;
	void mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux);
	void afficherJeuxDeveloppes();

private:
	std::pair<std::string, ListeJeux> paireNomJeux_;
};

//Kamil: Constructeur par defaut
Developpeur::Developpeur(const string& nom = "")
{
	paireNomJeux_.first = nom; 
	paireNomJeux_.second = {}; //Kamil: il faut initialiser la listeJeux de sorte qu'elle soit vide mais je suis un peu melange de cmt faire ca
}

//Kamil: Fonction pour obtenir le nom du developpeur
const std:: string& Developpeur:: obtenirNom() const
{
	return paireNomJeux_.first;
}

//Kamil: Fonction pour compter le nombre le jeux qu'un developpeur a developpes dans une ListeJeux quelconque
int Developpeur::compterNbJeux(const ListeJeux& listeJeux) const
{
	int nbJeux = 0;
	for (Jeu* ptrJeu : span(listeJeux.elements, listeJeux.nElements)) {
		if (ptrJeu->developpeur == obtenirNom())
			nbJeux++;
	}
	return nbJeux;
}

//Kamil: Fonction pour mettre a jour la liste de jeux developpes par un developpeur a partir de la liste de jeux de tous les jeux
void Developpeur::mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux)
{
	int tailleListeJeux = compterNbJeux(listeTousLesJeux);
	int index = 0;

	delete[] paireNomJeux_.second.elements;
	paireNomJeux_.second.elements = new Jeu * [tailleListeJeux];

	for (Jeu* ptrJeu : span(listeTousLesJeux.elements, listeTousLesJeux.nElements))
		if (ptrJeu->developpeur == obtenirNom()) {
			paireNomJeux_.second.elements[index++] = ptrJeu; // Leo: Warining pcq le compilateur a peur que l'index depasse la taille du tableau, mais normalement ca devrait etre correct (compterNbJeux)
		}
}

//Kamil: Fonction pour afficher les titres de tous les jeux developpes par un developpeur
void Developpeur::afficherJeuxDeveloppes()
{
	cout << "Les jeux développés par " << this->paireNomJeux_.first << "sont: ";
	for (Jeu* ptrJeu : span(paireNomJeux_.second.elements, paireNomJeux_.second.nElements))
		cout << ptrJeu->titre << endl;
}

Developpeur::~Developpeur()
{
	delete[] paireNomJeux_.second.elements;
}