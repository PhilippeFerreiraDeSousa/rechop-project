#include <map>
#include <vector>
#include <sstream>
#include "SharedElements.h"
using namespace std;

typedef map<int, Fenetre> MapFenetres;
typedef map <int, Volet> MapVolets;
typedef map<int, FenetreAvecVolet> MapSolutions;

int nbRabotsFenetre(FenetreAvecVolet const & solution, MapFenetres const & fenetres, MapVolets const & gauches, MapVolets const & droits, bool verbose){
	int result = 0;
	Fenetre fenetre = fenetres.find(solution.indexFenetre)->second;
	Volet gauche = gauches.find(solution.indexVoletGauche)->second;
	Volet droit = droits.find(solution.indexVoletDroit)->second;

	if (verbose) cout << "F:" << solution.indexFenetre << " G:" << solution.indexVoletGauche << " D:" << solution.indexVoletDroit << ", " ;

	//bloque en haut à gauche
	if (gauche.hauteurGondSommet > fenetre.hauteurGondSommetGauche ||  \
			gauche.hauteurInterGonds + gauche.hauteurGondSommet > fenetre.hauteurInterGondsGauche + fenetre.hauteurGondSommetGauche){
		++result;
		if (verbose) cout << " bloque haut gauche," ;
	}

	//bloque en bas à gauche
	if (gauche.hauteurTotale - gauche.hauteurGondSommet > fenetre.hauteurTotale - fenetre.hauteurGondSommetGauche && \
			gauche.hauteurTotale - gauche.hauteurGondSommet - gauche.hauteurInterGonds > \
				fenetre.hauteurTotale - fenetre.hauteurGondSommetGauche - fenetre.hauteurInterGondsGauche){
		++result;
		if (verbose) cout << " bloque bas gauche,";
	}

	//bloque en haut à droit
	if (droit.hauteurGondSommet > fenetre.hauteurGondSommetDroite ||  \
			droit.hauteurInterGonds + droit.hauteurGondSommet > fenetre.hauteurInterGondsDroite + fenetre.hauteurGondSommetDroite ){
		++result;
		if (verbose) cout << " bloque haut droit,";
	}

	//bloque en bas à droit
	if (droit.hauteurTotale - droit.hauteurGondSommet > fenetre.hauteurTotale - fenetre.hauteurGondSommetDroite && \
			droit.hauteurTotale - droit.hauteurGondSommet - droit.hauteurInterGonds > \
				fenetre.hauteurTotale - fenetre.hauteurGondSommetDroite - fenetre.hauteurInterGondsDroite){
		++result;
		if (verbose) cout << " bloque bas droit,";
	}
	// bloque au centre 
	if (gauche.largeur + droit.largeur > fenetre.largeurTotale)
	{
		++result;
		if (verbose) cout << "bloque centre,";
	}
	if (verbose) cout << endl;
	return result;
}


int evaluateSolution(MapSolutions const & solutions, MapFenetres const & fenetres, MapVolets const & gauches, MapVolets const & droits, bool verbose){
	int result = 0;
	for (auto it = solutions.begin(); it!= solutions.end(); ++it){
		result += nbRabotsFenetre(it->second, fenetres, gauches, droits, verbose);
	}
	return result;
}


int main(int argc, char *argv[]){
	if (argc != 5 && argc != 6){
		cout << "Syntax: " << endl;
		cout << "evaluateSolution fenetres.csv voletsGauches.csv voletsDroits.csv solution.csv (--verbose)" << endl;
		return 0;
	}
	string fenetresFile = argv[1];
	string gauchesFile = argv[2];
	string droitsFile = argv[3];
	string solutionsFile = argv[4];
	bool verbose = false;
	if (argc == 6){
		cout << argv[5] << endl;
		string verb(argv[5]);
		verbose = (verb == "--verbose");
	}

	MapFenetres fenetres;
	readCsvToMap<Fenetre>(fenetres, fenetresFile);

	MapVolets gauches;
	readCsvToMap<Volet>(gauches, gauchesFile);

	MapVolets droits;
	readCsvToMap<Volet>(droits, droitsFile);

	MapSolutions solutions;
	readCsvToMap(solutions, solutionsFile);

	auto nbBloquages = evaluateSolution(solutions, fenetres, gauches, droits, verbose);

	// cout << "Solution in " << solutionsFile << " costs " << nbBloquages << endl;
	cout << nbBloquages << endl;

	return 0;
}