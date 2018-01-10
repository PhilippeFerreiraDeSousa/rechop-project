#ifndef SHARED_ELEMENTS
#define SHARED_ELEMENTS

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;
#include <time.h> 
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

class Volet{
public:
	int index;
	int hauteurTotale;
	int hauteurGondSommet;
	int hauteurInterGonds;
	int largeur;

	int getIndex(){return index;}
	string classDescription(){
		return "index, hauteurTotale, hauteurGondSommet, hauteurInterGonds, largeur, ";
	}
	string toString(){
		stringstream result;
		result << index << ", ";
		result << hauteurTotale << ", ";
		result << hauteurGondSommet << ", ";
		result << hauteurInterGonds << ", ";
		result << largeur << ", ";
		return result.str();
	}
	Volet(){}
	Volet(string description){
		stringstream read(description);
		read >> index;
		read >> hauteurTotale;
		read >> hauteurGondSommet;
		read >> hauteurInterGonds;
		read >> largeur;	
	}
};


class Fenetre
{
public:
	int index;
	int hauteurTotale;
	int largeurTotale;
	int hauteurInterGondsGauche;
	int hauteurGondSommetGauche;
	int hauteurInterGondsDroite;
	int hauteurGondSommetDroite;

	int getIndex(){return index;}
	string classDescription(){
		stringstream result;
		result << "index, ";
		result << "hauteurTotale, ";
		result << "largeurTotale, ";
		result << "hauteurInterGondsGauche, ";
		result << "hauteurGondSommetGauche, ";
		result << "hauteurInterGondsDroite, ";
		result << "hauteurGondSommetDroite, ";
		return result.str();
	}
	string toString(){
		stringstream result;
		result << index << ", ";
		result << hauteurTotale << ", ";
		result << largeurTotale << ", ";
		result << hauteurInterGondsGauche << ", ";
		result << hauteurGondSommetGauche << ", ";
		result << hauteurInterGondsDroite << ", ";
		result << hauteurGondSommetDroite << ", ";
		return result.str();
	}
	Fenetre(){}
	Fenetre(string description){
		stringstream read(description);
		read >> index;
		read >> hauteurTotale;
		read >> largeurTotale;
		read >> hauteurInterGondsGauche;
		read >> hauteurGondSommetGauche;
		read >> hauteurInterGondsDroite;
		read >> hauteurGondSommetDroite;		
	}
};

class FenetreAvecVolet
{
public:
	int indexFenetre;
	int indexVoletGauche;
	int indexVoletDroit;

	int getIndex(){return indexFenetre;}

	string classDescription(){
		stringstream result;
		result << "indexFenetre, ";
		result << "indexVoletGauche, ";
		result << "indexVoletDroit, ";
		return result.str();
	}
	string toString(){
		stringstream result;
		result << indexFenetre << ", ";
		result << indexVoletGauche << ", ";
		result << indexVoletDroit << ", ";
		return result.str();
	}
	FenetreAvecVolet(){}
    FenetreAvecVolet(int idxFenetre, int idxVoletGauche, int idxVoletDroit){
        indexFenetre = idxFenetre;
        indexVoletGauche = idxVoletGauche;
        indexVoletDroit = idxVoletDroit;
    }
	FenetreAvecVolet(string description){
		stringstream read(description);
		read >> indexFenetre;
		read >> indexVoletGauche;
		read >> indexVoletDroit;
	}
};

void removeOneSubstring(string& s, string const & p);

template <typename Element>
void readCsvToMap(map<int,Element> & readMap, string filename){
    ifstream read(filename.c_str());
    if (!read.is_open()) throw;
    // cout << filename.c_str() << " read" << endl;
    string line;
	getline(read, line); // nbElements
    getline(read, line); // Description line
	while (getline(read, line)){
		removeOneSubstring(line, ",");

		Element elem(line);
		readMap.insert(pair<int, Element>(elem.getIndex(), elem));
	}
}

template <typename Element>
void printMapToCsv(map<int,Element> printedMap, string filename){
	ofstream write(filename.c_str());
	if (!write.is_open())
	{
		throw; 
	}
	write << "nbElements: " << printedMap.size() << endl;
	write << printedMap.begin()->second.classDescription() << endl;
	for (typename map<int,Element>::iterator it = printedMap.begin(); it!= printedMap.end(); ++it){
		write << it->second.toString() << endl;
	}
    // cout << filename.c_str() << " written" << endl;
}

#endif
