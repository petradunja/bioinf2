
#include "hmm.h"
#include "viterbi.h"
#include <iostream>
#include <vector>
#include <algorithm> //za max_element i distance


//vraca najvjerojatnija skrivena stanja - viterbi path
std::vector<std::string> viterbi(HMM& h, std::vector<std::string>& s) {

    
    int length = s.size(); //broj emitiranih parova simbola
    std::vector<std::vector<double>> viterbi_table(length, std::vector<double>(h.N, 0.0));
    std::vector<std::vector<int>> backpointer(length, std::vector<int>(h.N, 0));

 
    for (int t = 0; t < length; t++) {
        for (int state = 0; state < h.N; state++) {
            if (t == 0) {
                viterbi_table[t][state] = h.pi[state] * h.E[state][h.symbol_to_index[s[t]]];
            } else {
                //odredi najvjerojatnije prethodno stanje i vjerojatnost
                double max_prob = 0.0;
                int  max_prev_s;
                for (int prev_state = 0; prev_state < h.N; prev_state++) {
                    double current_prob = viterbi_table[t - 1][prev_state] * h.A[prev_state][state];
                    if (current_prob > max_prob) {
                        max_prob = current_prob;
                        max_prev_s = prev_state;
                    }
                }
                viterbi_table[t][state] = max_prob * h.E[state][h.symbol_to_index[s[t]]];
                backpointer[t][state] = max_prev_s;
            }
        }
    }
    for (const auto& row : viterbi_table) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        }

    for (const auto& row : backpointer) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        }

    //traceback
    double best_path_prob = *std::max_element(viterbi_table.back().begin(), viterbi_table.back().end()); //najveća vjerojatnost u zadnjem trenutku
    std::cout << best_path_prob << std::endl;
    int best_path_pointer = std::distance(viterbi_table.back().begin(), std::max_element(viterbi_table.back().begin(), viterbi_table.back().end())); //indeks predstavlja stanje koje odgovara najvećoj vjerojatnosti
    std::cout << best_path_pointer << std::endl;

    std::string best_path_state = h.states[best_path_pointer]; //zadnje skriveno stanje u najvjerojatnijem slijedu
    std::cout << best_path_state << std::endl;

    std::vector<std::string> best_path = {best_path_state};
    for (int t = length - 2; t >= 0; t--) {
        std::cout << best_path.front() << std::endl;
        int idx = backpointer[t][h.state_to_index[best_path.front()]];
        std::cout << idx << " " << std::endl;
        best_path.insert(best_path.begin(), h.states[idx]); 
    }

    
    return best_path;
}

int main() {
    	//TEST: POŠTENA/NEPOŠTENA KOCKA
	std::vector<std::string> s = {"5", "6", "6", "3", "2"};
	std::string filename2 = "model2.txt"; 
	HMM hmm2(filename2);
	
	hmm2.print();


	//VITERBI ALGORITHM
    //krivo mu je prvo stanje u usporedbi s R-om
	std::vector<std::string> path = viterbi(hmm2,s);
	for (auto& state : path) {
            std::cout << state << " ";
        }
    



	return 0;
}
