
#ifndef forward_H
#define forward_H

#include <iostream>
#include"hmm.h"
#include <cmath>
#include <vector>
#include <string>

using namespace std;

vector<vector<double>> forward(HMM& h, vector<string>& s){ //funkcija prima model i sekvencu emitiranih parova simbola
    
    int sequence_size = int(s.size());
    vector<vector<double>> alpha(h.N, vector<double>(sequence_size, 0.0));//matrica u  koju pohranjujem "unaprijedne" vjerojatnosti sekvenci

    for (int i = 0; i < h.N; i++)
    {
        alpha[i][0] = log(h.pi[i]) + log(h.E[i][h.symbol_to_index[s[0]]]);
    }

    for (int t = 1; t < sequence_size; t++) //vremenski trenuci
    {
        for (int i = 0; i < h.N; i++) //stanja
        {
            double logsum = -INFINITY;
            for (int j = 0; j < h.N; j++) {
                double log_term = alpha[j][t - 1] + log(h.A[j][i]);
                if(log_term > - INFINITY) {
                    logsum = log_term + log(1 + exp(logsum - log_term )); //NUMERIČKI STABILNIJE I PRECIZNIJE - rad s logaritmima - preuzeto iz R-a
                }
            }
            alpha[i][t] = logsum + log(h.E[i][h.symbol_to_index[s[t]]]);
        
        }
    }

    return alpha;
}

#endif

