// Auteurs: Leonard Pouliot (2150965) et Kamil Maarite (2152653)
// Date: 1 octobre 2022
// cours: INF1015

#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp" 
#include "Developpeur.hpp"
#include "ListeDeveloppeurs.hpp"

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
#define erreurFataleAssert(message) assert(false && (message)), terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;

size_t lireUintTailleVariable(istream& fichier)
{
    uint8_t entete = lireType<uint8_t>(fichier);
    switch (entete)
    {
    case enteteTailleVariableDeBase + 0:
        return lireType<uint8_t>(fichier);
    case enteteTailleVariableDeBase + 1:
        return lireType<uint16_t>(fichier);
    case enteteTailleVariableDeBase + 2:
        return lireType<uint32_t>(fichier);
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

Concepteur* chercherConcepteur(const string& nomConcepteur, const ListeJeux& listeJeux)
{
    Concepteur* ptrConcepteur = nullptr;
    for (Jeu* ptrJeu : spanListeJeux(listeJeux)) {
        for (Concepteur* tempPtrConcepteur : spanListeConcepteurs(ptrJeu->concepteurs))
        {
            if (tempPtrConcepteur->nom == nomConcepteur)
            {
                ptrConcepteur = tempPtrConcepteur;
            }
        }
    };
    return ptrConcepteur;
}

Concepteur* lireConcepteur(istream& fichier, const ListeJeux& listeJeux)
{
    Concepteur concepteur = {};
    concepteur.nom = lireString(fichier);
    concepteur.anneeNaissance = int(lireUintTailleVariable(fichier));
    concepteur.pays = lireString(fichier);

    Concepteur* ptrConcepteur = chercherConcepteur(concepteur.nom, listeJeux);

    if (ptrConcepteur == nullptr)
        ptrConcepteur = new Concepteur(concepteur);

    cout << "L'allocation du concepteur est réussie." << endl;

    return ptrConcepteur;
}

void changerTailleListeJeux(size_t nouvelleCapacite, ListeJeux& listeJeux)
{
    Jeu** nouvelleListeJeux = new Jeu* [nouvelleCapacite];

    for (size_t i = 0; i < listeJeux.nElements; i++)
        nouvelleListeJeux[i] = listeJeux.elements[i];

    delete[] listeJeux.elements;

    listeJeux.elements = nouvelleListeJeux;
    listeJeux.capacite = nouvelleCapacite;
}

void ajouterJeu(Jeu* jeu, ListeJeux& listeJeux)
{
    bool estListePleine = listeJeux.capacite - 1 <= listeJeux.nElements;
    bool estListeNouvelle = listeJeux.capacite == 0;

    if (estListePleine || estListeNouvelle )
    {
        size_t nouvelleCapacite = estListeNouvelle ? listeJeux.capacite = 1 : listeJeux.capacite * 2;
        changerTailleListeJeux(nouvelleCapacite, listeJeux);
    }

    listeJeux.elements[listeJeux.nElements] = jeu;
    listeJeux.nElements++;
}

void enleveJeuListe(Jeu* ptrJeu, ListeJeux& listeJeux)
{
    int indexJeu = -1;
    int nElements = listeJeux.nElements;

    for (size_t i = 0; i < nElements; i++)
    {
        if (listeJeux.elements[i] == ptrJeu)
            indexJeu = i;
    }

    bool estJeuPresent = indexJeu != -1;
    bool estListeVide = nElements  <= 0;

    if (estJeuPresent && !estListeVide)
    {
        Jeu* ptrDernierJeu = listeJeux.elements[nElements - 1];
        listeJeux.elements[indexJeu] = ptrDernierJeu;
        listeJeux.elements[nElements - 1] = nullptr;
        listeJeux.nElements--;
    }
}

Jeu* lireJeu(istream& fichier, const ListeJeux& listeJeux)
{
    Jeu jeu = {};
    jeu.titre = lireString(fichier);
    jeu.anneeSortie = int(lireUintTailleVariable(fichier));
    jeu.developpeur = lireString(fichier);
    jeu.concepteurs.nElements = lireUintTailleVariable(fichier);

    Jeu* ptrJeu = new Jeu(jeu);
    ptrJeu->concepteurs.elements = new Concepteur * [jeu.concepteurs.nElements];

    for ([[maybe_unused]] size_t i : iter::range(jeu.concepteurs.nElements))
    {
        Concepteur* ptrConcepteur = lireConcepteur(fichier, listeJeux);

        ptrJeu->concepteurs.elements[i] = ptrConcepteur;
        ajouterJeu(ptrJeu, ptrConcepteur->jeuxConcus);
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
        Jeu* ptrJeu = lireJeu(fichier, listeJeux);
        ajouterJeu(ptrJeu, listeJeux);
    }

    return listeJeux;
}


void detruireConcepteur(Concepteur* ptrConcepteur)
{
    cout << "Le pointeur du concepteur " << ptrConcepteur->nom << "a été detruit" << endl;
    delete[] ptrConcepteur->jeuxConcus.elements;
}


bool concepteurParticipeJeu(Jeu* ptrJeu, Concepteur* ptrConcepteur)
{
    bool estParticipantJeu = false;
    for (Concepteur* tempPtrConcepteur : spanListeConcepteurs(ptrJeu->concepteurs))
        if (tempPtrConcepteur == ptrConcepteur)
            estParticipantJeu = true;

    return estParticipantJeu;
}


void detruirejeu(Jeu* ptrJeu, ListeJeux& listeJeu)
{
    for (Concepteur* ptrConcepteur : spanListeConcepteurs(ptrJeu->concepteurs))
    {
        bool estDernierJeu = ptrConcepteur->jeuxConcus.nElements <= 1;
        if (estDernierJeu)
        {
            detruireConcepteur(ptrConcepteur);
            delete ptrConcepteur;
        }
        else
        {
            enleveJeuListe(ptrJeu, ptrConcepteur->jeuxConcus);
        }
    }

    cout << "Le pointeur du jeu " << ptrJeu->titre << " a été détruit." << endl;
    delete[] ptrJeu->concepteurs.elements;
}


void detruireListeJeux(ListeJeux& listeJeux)
{
    for (Jeu* ptrJeu : spanListeJeux(listeJeux))
    {
        detruirejeu(ptrJeu, listeJeux);
        delete ptrJeu;
    }
    delete[] listeJeux.elements;
}

void afficherConcepteur(const Concepteur& ptrConcepteur)
{
    cout << "\t" << ptrConcepteur.nom << ", " << ptrConcepteur.anneeNaissance << ", " << ptrConcepteur.pays << endl;
    // TODO REMOVE CE QUI EST EN DESSOUS
    for (Jeu* ptrJeu : spanListeJeux(ptrConcepteur.jeuxConcus))
    {
        cout << "\t\t" << ptrJeu->titre << ", " << ptrJeu->developpeur << ", " << ptrJeu->anneeSortie << endl;
    }
}

void afficherJeu(const Jeu* ptrJeu)
{
    cout << ptrJeu->titre << ", " << ptrJeu->developpeur << ", " << ptrJeu->anneeSortie << endl;
    cout << "Concepteurs: \n";

    for (Concepteur* ptrConcepteur : spanListeConcepteurs(ptrJeu->concepteurs))
    {
        afficherConcepteur(*ptrConcepteur);
    }
}

void afficherListeJeux(const ListeJeux& listeJeux)
{
    static const string ligneSeparation = "\n\033[12m--------------------------------------------------\033[0m\n";

    for (Jeu* ptrJeu : spanListeJeux(listeJeux))
    {
        afficherJeu(ptrJeu);
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

    // static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
    // ListeJeux listeJeux = creerListeJeux("jeux.bin");

    // cout << ligneSeparation;
    // cout << "Premier jeu de la liste :" << endl;
    // afficherJeu(listeJeux.elements[0]);
    // cout << ligneSeparation << endl;

    // cout << "Affichage de la liste de jeu:\n" << endl;
    // afficherListeJeux(listeJeux);

    // detruireListeJeux(listeJeux);

    // Tests sur les methodes des classes Developpeur et ListeDeveloppeurs
    ListeJeux listeJeuxTests = creerListeJeux("jeux.bin");

    Developpeur dev;
    cout << dev.obtenirNom() << endl;

    Developpeur square("Square");
    cout << square.obtenirNom() << endl;
    cout << square.compterNbJeux(listeJeuxTests) << endl; // Square a developpe 3 jeux
    square.mettreDeveloppeurAJour(listeJeuxTests);
    square.test();
    square.afficherJeuxDeveloppes();

    detruireListeJeux(listeJeuxTests);
}
