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

int deltaRabotGauche(const FenetreAvecVolet& solution1, const FenetreAvecVolet& solution2, const MapFenetres& fenetres, const MapVolets& gauches, const MapVolets& droits){
    return (nbRabotsFenetre(FenetreAvecVolet(solution1.indexFenetre, solution2.indexVoletGauche, solution1.indexVoletDroit), fenetres, gauches, droits, false)
          +nbRabotsFenetre(FenetreAvecVolet(solution2.indexFenetre, solution1.indexVoletGauche, solution2.indexVoletDroit), fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution1, fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution2, fenetres, gauches, droits, false));
}

int deltaRabotDroit(const FenetreAvecVolet& solution1, const FenetreAvecVolet& solution2, const MapFenetres& fenetres, const MapVolets& gauches, const MapVolets& droits){
    return (nbRabotsFenetre(FenetreAvecVolet(solution1.indexFenetre, solution1.indexVoletGauche, solution2.indexVoletDroit), fenetres, gauches, droits, false)
          +nbRabotsFenetre(FenetreAvecVolet(solution2.indexFenetre, solution2.indexVoletGauche, solution1.indexVoletDroit), fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution1, fenetres, gauches, droits, false)
          -nbRabotsFenetre(solution2, fenetres, gauches, droits, false));
}

int main(){
    // ########### Paramètres ############
    bool is_group_instance = false;
    // string instance = "petiteFaisable";
    // string instance = "petiteOpt";
    // string instance = "moyenneFaisable";
    // string instance = "moyenneOpt";
    // string instance = "grandeFaisable";
    // string instance = "grandeOpt";
    // string instance = "versaillesFaisable";
    string instance = "versaillesOpt";

    int init_time = 1;    // temps d'initialisation en secondes
    // int save_time = 60;     // interval denregistrement de la solution optimale
    int recuit_time = 600;
    int max_stuck_time = 20;     // temps maximum sans amélioration de la solution et sans réchauffement
    bool verbose = false;
    // ###################################

    srand (time(NULL));
    cout << "Instance : " << instance << (is_group_instance ? " (groupe 4) " : " (common) ") << endl;
    time_t now;
    time(&now);
    time_t stuck_time;
    int stuck_value;
    // time_t last_save_time;
    // time(&last_save_time);

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
    cout << "Initialisation :" << endl;
    for (int i=0; i<fenetres.size(); i++){
        sol_opt[i] = FenetreAvecVolet(i, i, i);
    }
    int eval_opt = evaluateSolution(sol_opt, fenetres, gauches, droits, false);
    int eval_curr = eval_opt;
    cout << eval_opt << " (init), " << endl;
    int init_step = 0;
    vector<int> indicesGauches;
    for (int i=0; i<N; i++) indicesGauches.push_back(i);
    vector<int> indicesDroits;
    for (int i=0; i<N; i++) indicesDroits.push_back(i);
    while ((int)(time(NULL)-now) < init_time) {  //  on tire des solutions au hasard
        random_shuffle(indicesGauches.begin(), indicesGauches.end());
        random_shuffle(indicesDroits.begin(), indicesDroits.end());
        for (int l=0; l<N; l++) {
            sol_curr[l] = FenetreAvecVolet(l, indicesGauches[l], indicesDroits[l]);
        }
        eval_curr = evaluateSolution(sol_curr, fenetres, gauches, droits, false);
        if (eval_curr < eval_opt) {
            sol_opt = sol_curr;
            eval_opt = eval_curr;
            cout << eval_curr << " (" << init_step << " its, " << time(NULL)-now << "s)" << endl;
        }

        init_step++;
    }
    cout << "Sur " << init_step << " essais en " << init_time << " s" << endl << endl;
    sol_curr = sol_opt;
    eval_curr = eval_opt;
    time(&now);
    time(&stuck_time);
    // Recuit simulé
    cout << "Recuit simulé :" << endl;
    int recuit_step = 0;
    int temp_step = 0;  // nombres d'itérations du cycle en cours
    int temp_stage = 0; // nombre de cycles thermiques
    int n, m, p, q, deltaGauche, deltaDroit;  // différence de coups de rabot après transposition des volets gauches, des volets droits et depuis la solution optimale
    double proba;
    while ((int)(time(NULL)-now) < recuit_time) {
        if ((int)(time(NULL)-stuck_time) > max_stuck_time) {
            time(&stuck_time);
            temp_step = 0;
            temp_stage++;
        }
        T = 2./(1.+0.001*temp_step+0.3*temp_stage);

        do {
            n = rand() % N; //  les voisins de notre permutation courante sont distantes d'une transposition (distance dans le groupe symétrique : nb de transpositions entre)
            m = rand() % N;
        } while (n == m);
        deltaGauche = deltaRabotGauche(sol_curr[n], sol_curr[m], fenetres, gauches, droits);
        proba = (double)min(1., (double)exp(-(double)(deltaGauche/T)));
        // cout << "deltaGauche : " << deltaGauche << endl;
        if (proba>(double)rand()/RAND_MAX){
            transposeVoletsGauche(sol_curr, n, m);
            eval_curr += deltaGauche;
            if (eval_curr-eval_opt < 0) {
                sol_opt = sol_curr;
                eval_opt = eval_curr;
                time(&stuck_time);
                stuck_value = eval_curr;
                stringstream nbBloquages;
                nbBloquages << eval_opt;
                string solutionsFile = "../output/solutions/"+instance+"_sol/"+nbBloquages.str()+".csv";
                printMapToCsv(sol_opt, solutionsFile);
                cout << eval_opt << " (" << recuit_step << " its, " << time(NULL)-now << "s) - nouvelle solution optimale" << endl;
            } else if (stuck_value != eval_curr){
                time(&stuck_time);
                stuck_value = eval_curr;
            } //else {
                //cout << eval_curr << " (" << recuit_step << " its, " << time(NULL)-now << "s) - volets gauches "<< n << " et " << m << " permutés" << " (T : " << T << ", P : " << proba << ")" << endl;
            //}
            // cout << "vrai : " << evaluateSolution(sol_curr, fenetres, gauches, droits, false) << endl;
            // cout << recuit_step << " " << T << " " << proba << endl;
        }
        // cout << eval_curr << " (" << recuit_step << " its, " << time(NULL)-now << "s) - volets gauches "<< n << " et " << m << " pas permutés" << endl;
        do {
            p = rand() % N;
            q = rand() % N;
        } while (p == q);
        deltaDroit = deltaRabotDroit(sol_curr[p], sol_curr[q], fenetres, gauches, droits);
        // cout << "deltaDroit : " << deltaDroit << endl;
        proba = min(1., (double)exp(-(double)(deltaDroit/T)));
        if (proba>(double)rand()/RAND_MAX){
            transposeVoletsDroit(sol_curr, p, q);
            eval_curr += deltaDroit;
            if (eval_curr-eval_opt < 0) {
                sol_opt = sol_curr;
                eval_opt = eval_curr;
                time(&stuck_time);
                stuck_value = eval_curr;
                stringstream nbBloquages;
                nbBloquages << eval_opt;
                string solutionsFile = "../output/solutions/"+instance+"_sol/"+nbBloquages.str()+".csv";
                printMapToCsv(sol_opt, solutionsFile);
                cout << eval_opt << " (" << recuit_step << " its, " << time(NULL)-now << "s) - nouvelle solution optimale" << endl;
            } else if (stuck_value != eval_curr){
                time(&stuck_time);
                stuck_value = eval_curr;
            } //else {
                //cout << eval_curr << " (" << recuit_step << " its, " << time(NULL)-now << "s) - volets droits "<< p << " et " << q << " permutés" << " (T : " << T << ", P : " << proba << ")" << endl;
            //}
            // cout << "vrai : " << evaluateSolution(sol_curr, fenetres, gauches, droits, false) << endl;
            // cout << recuit_step << " " << T << " " << proba << endl;
        }
        // cout << eval_curr << " (" << recuit_step << " its, " << time(NULL)-now << "s) - volets droits "<< p << " et " << q << " pas permutés" << endl;
        recuit_step++;
        temp_step++;
    }
    cout << "Sur " << recuit_step << " essais en " << recuit_time << " s" << endl;
    cout << "Nombre de cycles thermiques : " << temp_stage << endl;

    stringstream nbBloquages;
    nbBloquages << evaluateSolution(sol_opt, fenetres, gauches, droits, verbose);
    string solutionsFile = "../output/solutions/"+instance+"_sol/"+nbBloquages.str()+".csv";
    printMapToCsv(sol_opt, solutionsFile);
    cout << "Solution in " << solutionsFile << " costs " << nbBloquages.str() << endl;
    // cout << nbBloquages << endl;

	return 0;
}
