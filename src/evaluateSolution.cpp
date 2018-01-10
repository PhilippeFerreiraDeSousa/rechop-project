#include <map>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "SharedElements.h"
using namespace std;

typedef map<int, Fenetre> MapFenetres;
typedef map <int, Volet> MapVolets;
typedef map<int, FenetreAvecVolet> MapSolutions;

int nbRabotsFenetre(FenetreAvecVolet const & solution, MapFenetres const & fenetres, MapVolets const & gauches, MapVolets const & droits, bool verbose){
	int result = 0;
    MapFenetres::const_iterator itFen = fenetres.find(solution.indexFenetre);
    if (itFen == fenetres.end()) {
        cout << endl << "Erreur d'indice fenêtre : " << solution.indexFenetre << endl;
    }
    Fenetre fenetre = itFen->second;

    MapVolets::const_iterator itGauche = gauches.find(solution.indexVoletGauche);
    if (itGauche == gauches.end()) {
        cout << endl << "Erreur d'indice volet gauche : " << solution.indexVoletGauche << endl;
    }
    Volet gauche = itGauche->second;

    MapVolets::const_iterator itDroit = droits.find(solution.indexVoletDroit);
    if (itDroit == droits.end()) {
        cout << endl << "Erreur d'indice volet droit : " << solution.indexVoletDroit << endl;
    }
    Volet droit = itDroit->second;

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

void transposeVoletsDroit(MapSolutions& solutions, int n, int m) {
    int indexVoletDroit = solutions[n].indexVoletDroit;
    solutions[n].indexVoletDroit = solutions[m].indexVoletDroit;
    solutions[m].indexVoletDroit = indexVoletDroit;
}

void transposeVoletsGauche(MapSolutions& solutions, int n, int m) {
    int indexVoletGauche = solutions[n].indexVoletGauche;
    solutions[n].indexVoletGauche = solutions[m].indexVoletGauche;
    solutions[m].indexVoletGauche = indexVoletGauche;
}

void transposeFenetre(MapSolutions& solutions, int n, int m) {
    int indexFenetre = solutions[n].indexFenetre;
    solutions[n].indexFenetre = solutions[m].indexFenetre;
    solutions[m].indexFenetre = indexFenetre;
}

int deltaRabot(const FenetreAvecVolet& solution1, const FenetreAvecVolet& solution2, const MapFenetres& fenetres, const MapVolets& gauches, const MapVolets& droits){
    return nbRabotsFenetre(FenetreAvecVolet(solution1.indexFenetre, solution2.indexVoletGauche, solution1.indexVoletDroit), fenetres, gauches, droits, false)
          +nbRabotsFenetre(FenetreAvecVolet(solution2.indexFenetre, solution1.indexVoletGauche, solution2.indexVoletDroit), fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution1, fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution2, fenetres, gauches, droits, false);
}

int main(){
    // ########### Paramètres ############
    bool is_group_instance = false;
    string instance = "petiteFaisable";
    int init_time = 10;     // temps d'initialisation en secondes
    int save_time = 60;     // interval denregistrement de la solution optimale
    int recuit_times = 10000;
    bool verbose = true;
    // ###################################

    srand (time(NULL));
    cout << "Instance : " << instance << (is_group_instance ? " (groupe 4) " : " (common) ") << endl;
    time_t now;
    time(&now);

    string prefix = is_group_instance ? "group4/" : "common/";
    string fenetresFile = "../input/"+prefix+instance+"_fenetres.csv";
    string gauchesFile = "../input/"+prefix+instance+"_voletsDroits.csv";
    string droitsFile = "../input/"+prefix+instance+"_voletsGauches.csv";
	MapFenetres fenetres;
    readCsvToMap<Fenetre>(fenetres, fenetresFile);
	MapVolets gauches;
	readCsvToMap<Volet>(gauches, gauchesFile);
	MapVolets droits;
	readCsvToMap<Volet>(droits, droitsFile);
    MapSolutions sol_opt, sol_curr;

    int N = fenetres.size();
    cout << "Nombre de fenetres : " << N << endl;
    double T;    // temperature

    // initialisation
    cout << "Initialisations : ";
    for (int i=0; i<fenetres.size(); i++){
        sol_opt[i] = FenetreAvecVolet(i, i, i);
    }
    int eval_opt = evaluateSolution(sol_opt, fenetres, gauches, droits, false);
    cout << eval_opt << " (init), ";
    int init_step = 0;
    while (time(NULL)-now < init_time) {  //  on tire des solutions au hasard
        vector<int> indicesGauches;
        for (int i=0; i<N; i++) indicesGauches.push_back(i);
        random_shuffle(indicesGauches.begin(), indicesGauches.end());
        vector<int> indicesDroits;
        for (int i=0; i<N; i++) indicesDroits.push_back(i);
        random_shuffle(indicesDroits.begin(), indicesDroits.end());
        for (int l=0; l<N; l++) {
            sol_curr[l] = FenetreAvecVolet(l, indicesGauches[l], indicesDroits[l]);
        }
        int eval_curr = evaluateSolution(sol_curr, fenetres, gauches, droits, false);
        if (eval_curr < eval_opt) {
            sol_opt = sol_curr;
            eval_opt = eval_curr;
            cout << eval_curr << " (" << init_step << "e), ";
        }
        init_step++;
    }
    cout << " (sur " << init_step << " essais en " << init_time << " s)" << endl;

    // Recuit simulé
    cout << "Recuit simulé : ";
    for (int k=0; k<recuit_times; k++) {  //  on initialise la solution courrante à la permutation identité
        T = 1./(1.+k);
        int n = rand() % N; //  les voisins de notre permutation courante sont distantes d'une transposition (distance dans le groupe symétrique : nb de transpositions entre)
        int m = rand() % N;
        int p = rand() % N;
        int q = rand() % N;
        double proba = min(1., (double)exp(-deltaRabot(sol_curr[n], sol_curr[m], fenetres, gauches, droits)/T));
        if (proba>(double)rand()/RAND_MAX){
            transposeVoletsDroit(sol_curr, n, m);
            transposeVoletsGauche(sol_curr, p, q);
            int eval_curr = evaluateSolution(sol_curr, fenetres, gauches, droits, false);
            if (eval_curr < eval_opt) {
                sol_opt = sol_curr;
                eval_opt = eval_curr;
                cout << eval_curr << " (" << k << "e), ";
            }
            // cout << k << " " << T << " " << proba << endl;
        }
    }
    cout << " (sur " << recuit_times << " essais)" << endl;

    stringstream nbBloquages;
    nbBloquages << evaluateSolution(sol_opt, fenetres, gauches, droits, verbose);
    string solutionsFile = "../output/solutions/"+instance+"_sol/"+nbBloquages.str()+".csv";
    printMapToCsv(sol_opt, solutionsFile);
    cout << "Solution in " << solutionsFile << " costs " << nbBloquages.str() << endl;
    // cout << nbBloquages << endl;

	return 0;
}
