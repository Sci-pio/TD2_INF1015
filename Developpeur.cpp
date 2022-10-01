#include "Developpeur.hpp"
#include "gsl/span"
#include <iostream>
using namespace gsl;
using namespace gsl;


// Constructeur par defaut
Developpeur::Developpeur(const string& nom = "")
{
	paireNomJeux_.first = nom;
	paireNomJeux_.second = {};
}

// Fonction pour obtenir le nom du developpeur
const string& Developpeur::obtenirNom() const
{
	return paireNomJeux_.first;
}

// Fonction pour compter le nombre le jeux qu'un developpeur a developpes dans une ListeJeux quelconque
int Developpeur::compterNbJeux(const ListeJeux& listeJeux) const
{
	int nbJeux = 0;
	for (Jeu* ptrJeu : span(listeJeux.elements, listeJeux.nElements)) {
		if (ptrJeu->developpeur == obtenirNom())
			nbJeux++;
	}
	return nbJeux;
}

// Fonction pour mettre a jour la liste de jeux developpes par un developpeur a partir de la liste de jeux de tous les jeux
void Developpeur::mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux)
{
	int tailleListeJeux = compterNbJeux(listeTousLesJeux);
	int index = 0;

	delete[] paireNomJeux_.second.elements;
	paireNomJeux_.second.elements = new Jeu * [tailleListeJeux];

	for (Jeu* ptrJeu : span(listeTousLesJeux.elements, listeTousLesJeux.nElements))
		if (ptrJeu->developpeur == obtenirNom()) {
			paireNomJeux_.second.elements[index++] = ptrJeu; // Leo: Warning pcq le compilateur a peur que l'index depasse la taille du tableau, mais normalement ca devrait etre correct (compterNbJeux)
		}
}

// Fonction pour afficher les titres de tous les jeux developpes par un developpeur
void Developpeur::afficherJeuxDeveloppes()
{
	cout << "Les jeux développés par " << paireNomJeux_.first << "sont: " << endl;
	for (Jeu* ptrJeu : span(paireNomJeux_.second.elements, paireNomJeux_.second.nElements))
		cout << ptrJeu->titre << endl;
}

// Destructeur
Developpeur::~Developpeur()
{
	delete[] paireNomJeux_.second.elements;
}