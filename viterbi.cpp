
#include "hmm.h"
#include "viterbi.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> //za max_element i distance

using namespace std;

//vraca najvjerojatnija skrivena stanja - viterbi path
vector<string> viterbi(HMM& h, vector<string>& s) {

    
    int length = s.size(); //broj emitiranih parova simbola
    vector<vector<double>> viterbi_table(length, vector<double>(h.N, 0.0));
    vector<vector<int>> backpointer(length, vector<int>(h.N, 0));
 
    for (int t = 0; t < length; t++) {
        for (int state = 0; state < h.N; state++) {
            if (t == 0) {
                viterbi_table[t][state] = log(h.pi[state] * h.E[state][h.symbol_to_index[s[t]]]);
                backpointer[t][state] = state;
            } else {
                //odredi najvjerojatnije prethodno stanje i vjerojatnost
                double max_prob = - INFINITY;
                int  max_prev_s = -1;
                for (int prev_state = 0; prev_state < h.N; prev_state++) {
                    double current_prob = viterbi_table[t - 1][prev_state] + log(h.A[prev_state][state]);
                    if (current_prob > max_prob) {
                        max_prob = current_prob;
                        max_prev_s = prev_state;
                    }
                }
                viterbi_table[t][state] = max_prob + log(h.E[state][h.symbol_to_index[s[t]]]);
                backpointer[t][state] = max_prev_s;
            }
        }
    }
    std::cout << "viterbi table" << endl;
    for (const auto& row : viterbi_table) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << endl;
    }
    std::cout << "backpointer" << endl;
    for (const auto& row : backpointer) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << endl;
    }

    //traceback
    double best_path_prob = *max_element(viterbi_table.back().begin(), viterbi_table.back().end()); //najveća vjerojatnost u zadnjem trenutku
    std::cout << "Best path prob = " << best_path_prob << endl;
    int best_path_pointer = distance(viterbi_table.back().begin(), max_element(viterbi_table.back().begin(), viterbi_table.back().end())); //indeks predstavlja stanje koje odgovara najvećoj vjerojatnosti
    std::cout << best_path_pointer << endl;

    string best_path_state = h.states[best_path_pointer]; //zadnje skriveno stanje u najvjerojatnijem slijedu
    std::cout << "Best path state = " << best_path_state << endl;

    vector<string> best_path = {best_path_state};
    for (int t = length - 1; t > 0; t--) {
        int idx = backpointer[t][h.state_to_index[best_path.front()]];
        best_path.insert(best_path.begin(), h.states[idx]); 
    }
    return best_path;
}
