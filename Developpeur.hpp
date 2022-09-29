#pragma once
#include <string>
#include "ListeJeux.hpp"
#include "main.cpp" //Kamil: Je fais include le main pour avoir acces a la fonction spanListeJeux()

class Developpeur
{
	//TODO: En faire une classe qui suit les principes OO.
	//TODO: La destruction d'un Developpeur doit s'assurer que la désallocation de ListeJeux est faite.
	//TODO: Les méthodes à faire...
	
public:
	Developpeur(const std:: string& nom, const ListeJeux& listeJeuxDeveloppes);
	const std::string& obtenirNom() const;
	int compterNbJeux(const ListeJeux& listeJeux) const;
	void mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux);
	void afficherJeuxDeveloppes();

private:
	std::pair<std::string, ListeJeux> paireNomJeux_;
};

//Kamil: Constructeur par defaut
Developpeur::Developpeur(const std::string& nom, const ListeJeux& listeJeuxDeveloppes)
{
	paireNomJeux_.first = nom; 
	paireNomJeux_.second = listeJeuxDeveloppes; //Kamil: il faut initialiser la listeJeux de sorte qu'elle soit vide mais je suis un peu melange de cmt faire ca
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
	for (Jeu* ptrJeu : spanListeJeux(listeJeux)) {
		if (ptrJeu->developpeur == this->obtenirNom())
			nbJeux++;
	}
	return nbJeux;
}

//Kamil: Fonction pour mettre a jour la liste de jeux developpes par un developpeur a partir de la liste de jeux de tous les jeux
void Developpeur::mettreDeveloppeurAJour(ListeJeux& listeTousLesJeux)
{
	int tailleListeJeux = this->compterNbJeux(listeTousLesJeux);
	while (listeTousLesJeux.nElements <= tailleListeJeux) {
		for (Jeu* ptrJeu : spanListeJeux(listeTousLesJeux))
			if (ptrJeu->developpeur == this->obtenirNom())
				ajouterJeu(ptrJeu, paireNomJeux_.second);
	}

}

//Kamil: Fonction pour afficher les titres de tous les jeux developpes par un developpeur
void Developpeur::afficherJeuxDeveloppes()
{
	cout << "Les jeux développés par " << this->paireNomJeux_.first << "sont: ";
	for (Jeu* ptrJeu : spanListeJeux(paireNomJeux_.second))
		cout << ptrJeu->titre << endl;
}