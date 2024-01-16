
#ifndef hmm_h
#define hmm_h

#include<fstream> 
#include <vector> 
#include <map>  
#include <utility>
#include <string>

using std::fstream;

class HMM
{
public:
	HMM(std::string& filename);	// konstruktor za inicijalizaciju modela iz datoteke
	
	void print(); //ispis parametara modela
	//friend funkcje
	friend std::vector<std::vector<double>>  forward(HMM& h, std::vector<std::string>& s); // vjerojatnost da je model u trenutku t u stanju i 
	friend std::vector<std::vector<double>>  backward(HMM& h, std::vector<std::string>& s); // vjerojatnost da je model u trenutku t u stanju i i da je emitirao T - t simbola (u trenutcima T do t + 1)
	friend HMM baumWelch(HMM& h, std::vector<std::string>& s, double delta, int num_iter); // Baum-Welch algoritam
	friend std::vector<std::string> viterbi(HMM& h, std::vector<std::string>& s); // Viterbi algoritam
	friend std::pair<std::string, std::string> viterbi3(const std::string& x, const std::string& y,HMM& h); // Viterbi algoritam za pairwise sequence alignment



public:
	int N;	// broj skrivenih stanja
	int M;	// broj mogućih opservacija (simbola)

	std::vector<std::string> symbols;  //simboli - emitirani parovi (mora biti pointer radi brisanja u destruktoru)
    std::vector<std::string> states;   //skrivena stanja
    std::map<std::string, int> state_to_index;  // mapiranje stanja na indekse
    std::map<std::string, int> symbol_to_index; // mapiranje simbola na indekse

	std::vector<std::vector<double>> A; // MATRICA PRIJELAZNIH VJEROJATNOSTI, a[i][j] je vjerojatnost prijelaza iz stanja i u stanje j 
	std::vector<std::vector<double>> E; // MATRICA EMISIJE, e[i][j] je vjerojatnost emisije simbola j u stanju i 
	std::vector<double> pi; // MATRICA POČENTIH VJEROJATNOSTI, pi[i] je vjerojatnost da je model u stanju i u trenutku 0

	//double **A;
	//double **E;	
	//double *pi;	
	
};

#endif 