﻿#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.

using namespace std;
using namespace iter;
using namespace gsl;

#pragma region "Fonctions de base pour vous aider"
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;

size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase + 0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

gsl::span<Jeu*> spanListeJeux(const ListeJeux& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}

gsl::span<Concepteur*> spanListeConcepteurs(const ListeConcepteurs& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}

#pragma endregion

//TODO: Fonction qui cherche un concepteur par son nom dans une ListeJeux.
// Cette fonction renvoie le pointeur vers le concepteur si elle le trouve dans
// un des jeux de la ListeJeux. En cas contraire, elle renvoie un pointeur nul.
//Kamil: Des erreurs de type que je comprends pas trop
Concepteur* chercherConcepteur(const string& nomConcepteur, const ListeJeux& listeJeux) {
	Concepteur* ptrConcepteur = nullptr;
	for (Jeu* ptrJeu : spanListeJeux(listeJeux)) {
		for (Concepteur* ptrConcepteur2 : spanListeConcepteurs(ptrJeu->concepteurs)) {
			if (ptrConcepteur2->nom == nomConcepteur) {
				ptrConcepteur = ptrConcepteur2;
			}
		}
	};
	return ptrConcepteur;
}


Concepteur* lireConcepteur(istream& fichier, const ListeJeux& listeJeux)
{
	Concepteur concepteur = {}; // On initialise une structure vide de type Concepteur.
	concepteur.nom = lireString(fichier);
	concepteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	concepteur.pays = lireString(fichier);
	// Rendu ici, les champs précédents de la structure concepteur sont remplis
	// avec la bonne information.

	Concepteur* ptrConcepteur;
	bool existeDeja = false;

	while (!existeDeja) {
		for (Jeu* ptrJeu : spanListeJeux(listeJeux))
			for (Concepteur* ptrConcepteur2 : spanListeConcepteurs(ptrJeu->concepteurs))
				if (ptrConcepteur2->nom == concepteur.nom) {
					existeDeja = true;
					ptrConcepteur = ptrConcepteur2;
				}
	}

	if (existeDeja == false) {
		Concepteur* ptrConcepteur = new Concepteur;
		ptrConcepteur->nom = concepteur.nom;
		ptrConcepteur->anneeNaissance = concepteur.anneeNaissance;
		ptrConcepteur->pays = concepteur.pays;
	}

	return ptrConcepteur;
	cout << "L'allocation du concepteur est réussie." << endl;
	
}


void changerTailleListeJeux(size_t nouvelleCapacite, ListeJeux& listeJeux) {
	//allouer nouveau tableau de capacite nouvelleCapacite
	Jeu** nouvelleListeJeux = new Jeu * [nouvelleCapacite];

	//Copie des adresse des éléments de l'ancienne liste dans la nouvelle liste
	for (size_t i = 0; i < listeJeux.nElements; i++)
		nouvelleListeJeux[i] = listeJeux.elements[i];

	//Delete de l'ancien espace mémoire des élements
	delete[] listeJeux.elements;

	listeJeux.elements = nouvelleListeJeux;
	listeJeux.capacite = nouvelleCapacite;
}


void ajouterJeu(Jeu* jeu, ListeJeux& listeJeux)
{
	if (listeJeux.capacite - 1 <= listeJeux.nElements || listeJeux.capacite == 0)
	{
		size_t nouvelleCapacite = listeJeux.capacite == 0 ? listeJeux.capacite = 1 : listeJeux.capacite * 2;
		changerTailleListeJeux(nouvelleCapacite, listeJeux);
	}

	listeJeux.elements[listeJeux.nElements] = jeu;
	listeJeux.nElements++;
}

//TODO: Fonction qui enlève un jeu de ListeJeux.
// Attention, ici il n'a pas de désallocation de mémoire. Elle enlève le
// pointeur de la ListeJeux, mais le jeu pointé existe encore en mémoire.
// Puisque l'ordre de la ListeJeux n'a pas être conservé, on peut remplacer le
// jeu à être retiré par celui présent en fin de liste et décrémenter la taille
// de celle-ci.

Jeu* lireJeu(istream& fichier)
{
	Jeu jeu = {}; // On initialise une structure vide de type Jeu
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = int(lireUintTailleVariable(fichier));
	jeu.developpeur = lireString(fichier);
	jeu.concepteurs.nElements = lireUintTailleVariable(fichier);
	// Rendu ici, les champs précédents de la structure jeu sont remplis avec la
	// bonne information.

	Jeu* ptrJeu = new Jeu;
	ptrJeu->titre = jeu.titre;
	ptrJeu->anneeSortie = jeu.anneeSortie;
	ptrJeu->developpeur = jeu.developpeur;
	ptrJeu->concepteurs.nElements = jeu.concepteurs.nElements;
	ptrJeu->concepteurs.elements = new Concepteur * [jeu.concepteurs.nElements];

	for ([[maybe_unused]] size_t i : iter::range(jeu.concepteurs.nElements)) {
		Concepteur* ptrConcepteur = lireConcepteur(fichier);
		ptrJeu->concepteurs.elements[i] = ptrConcepteur;

		//TODO: Ajouter le jeu à la liste des jeux auquel a participé le concepteur.
	}
	return ptrJeu;
}

ListeJeux creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	size_t nElements = lireUintTailleVariable(fichier);
	ListeJeux listeJeux = {};


	for ([[maybe_unused]] size_t n : iter::range(nElements))
	{
		Jeu* jeu = lireJeu(fichier);
		ajouterJeu(jeu, listeJeux);
	}

	return listeJeux;
}

//TODO: Fonction pour détruire un concepteur (libération de mémoire allouée).
// Lorsqu'on détruit un concepteur, on affiche son nom pour fins de débogage.
void deleteConcepteur(Concepteur* ptrConcepteur) {
	cout << "Le pointeur du Concepteur" << ptrConcepteur->nom << "a ete detruit";
	delete[] ptrConcepteur;
}

//TODO: Fonction qui détermine si un concepteur participe encore à un jeu.
bool concepteurParticipeJeu(Jeu* ptrJeu, const string nomConcepteur) {
	bool concepteurParticipeJeu = true;
	//Kamil: je vais attendre de comprendre comment les concepteurs sont enleves d'un jeu avant d'ecrire la fonction

	return concepteurParticipeJeu;
}

//TODO: Fonction pour détruire un jeu (libération de mémoire allouée).
// Attention, ici il faut relâcher toute les cases mémoires occupées par un jeu.
// Par conséquent, il va falloir gérer le cas des concepteurs (un jeu contenant
// une ListeConcepteurs). On doit commencer par enlever le jeu à détruire des jeux
// qu'un concepteur a participé (jeuxConcus). Si le concepteur n'a plus de
// jeux présents dans sa liste de jeux participés, il faut le supprimer.  Pour
// fins de débogage, affichez le nom du jeu lors de sa destruction.

//TODO: Fonction pour détruire une ListeJeux et tous ses jeux.

void afficherConcepteur(const Concepteur& d)
{
	cout << "\t" << d.nom << ", " << d.anneeNaissance << ", " << d.pays
		<< endl;
}

//TODO: Fonction pour afficher les infos d'un jeu ainsi que ses concepteurs.
// Servez-vous de la fonction afficherConcepteur ci-dessus.
void afficherJeu(const Jeu* jeu) {
	cout << jeu->titre << ", " << jeu->developpeur << ", " << jeu->anneeSortie << endl;
	cout << "Concepteurs: \n";
	
	for (int i = 0; i < jeu->concepteurs.nElements; i++)
	{	
		afficherConcepteur(*jeu->concepteurs.elements[i]);
	}
}

//TODO: Fonction pour afficher tous les jeux de ListeJeux, séparés par un ligne.
// Servez-vous de la fonction d'affichage d'un jeu crée ci-dessus. Votre ligne
// de séparation doit être différent de celle utilisée dans le main.
void afficherListeJeux(const ListeJeux& listeJeux) {
	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	
	for (int i = 0; i < listeJeux.nElements; i++)
	{
		afficherJeu(listeJeux.elements[i]);
		cout << ligneSeparation;
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	ListeJeux listeJeux = creerListeJeux("jeux.bin"); //TODO: Appeler correctement votre fonction de création de la liste de jeux.

	
	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.
	afficherListeJeux(listeJeux);
	cout << "Premier jeu de la liste :" << endl;


	//TODO: Appel à votre fonction d'affichage de votre liste de jeux.

	//TODO: Faire les appels à toutes vos fonctions/méthodes pour voir qu'elles fonctionnent et avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//TODO: Détruire tout avant de terminer le programme.  Devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
}
