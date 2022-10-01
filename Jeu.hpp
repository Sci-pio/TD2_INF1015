#pragma once
#include <string>
#include "ListeConcepteurs.hpp"

struct Jeu
{
	std::string titre;
	int anneeSortie;
	std::string developpeur; // Peut etre mettre un Developpeur developpeur?
	ListeConcepteurs concepteurs;
};
