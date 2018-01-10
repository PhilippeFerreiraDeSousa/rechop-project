#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


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
	FenetreAvecVolet(string description){
		stringstream read(description);
		read >> indexFenetre;
		read >> indexVoletGauche;
		read >> indexVoletDroit;
	}
};


void removeOneSubstring(string& s, string const & p) { 
  string::size_type n = p.length();
  for (string::size_type i = s.find(p);
      i != string::npos;
      i = s.find(p))
      s.erase(i, n);
}

map<int,int> readWidthFile(string filename){	
	ifstream read(filename.c_str());
	if (!read.is_open())
	{
		cerr << "file " << filename << " does not exist"<< endl;
	}
		
	map<int, int> result;

	string line;
	getline(read, line); // nbElements
	getline(read, line); // Description line
	while (getline(read, line)){
		removeOneSubstring(line, ",");
		stringstream read(line);
		pair<int, int> indexWidth;
		read >> indexWidth.first;
		read >> indexWidth.second;
		result.insert(indexWidth);
	}
	return result;
}

template <typename Element>
void readCsvToMap(map<int,Element> & readMap, string filename){
	ifstream read(filename.c_str());
	if (!read.is_open()) throw;
	string line;
	getline(read, line); // nbElements
	getline(read, line); // Description line
	while (getline(read, line)){
		removeOneSubstring(line, ",");

		Element elem(line);
		readMap.insert(pair<int, Element>(elem.getIndex(), elem));
	}
}




int main(int argc, char *argv[])
{
    
	if (argc != 5 && argc != 6){
		cout << "Syntax: " << endl;
		cout << "evaluateLargeur fenetres.csv voletsGauches.csv voletsDroits.csv solution.csv (--verbose)" << endl;
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

    string propertiesFile(argv[1]);
    string instanceFile(argv[2]);

    map<int,int> fenetres = readWidthFile(fenetresFile);
    map<int,int> voletsGauches = readWidthFile(gauchesFile);
    map<int,int> voletsDroits = readWidthFile(droitsFile);
    map<int,FenetreAvecVolet> solutions;
	readCsvToMap(solutions, solutionsFile);

	int nbBloquages = 0;

	for (auto it= solutions.begin(); it!= solutions.end(); ++it){
		auto gauche = voletsGauches.find(it->second.indexVoletGauche)->second;
		auto droite = voletsDroits.find(it->second.indexVoletDroit)->second;
		auto fenetre = fenetres.find(it->second.indexFenetre)->second;
		bool bloque = (fenetre < gauche + droite);
		nbBloquages +=  bloque;
		if (verbose) {
			cout << "F " << it->second.indexFenetre << " ( " << fenetre << " ), ";
			cout << " G " << it->second.indexVoletGauche << " ( " << gauche << " ), ";
			cout << " D " << it->second.indexVoletDroit << " ( " << droite << " )";
			if (bloque) cout << " bloquent";
			cout << endl;
		}
	}

	cout << nbBloquages << endl;


	return 0;
}