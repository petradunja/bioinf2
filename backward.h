#ifndef backward_H
#define backward_H

#include"hmm.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
//vjerojatnost da je model u trenutku t u stanju i da će emitirati preostale simbole u trenutcim t+1 do T

vector<vector<double>> backward(HMM& h, vector<string>& s){ //funkcija prima model i sekvencu emitiranih parova simbola
    
    int sequence_size = int(s.size());
    vector<vector<double>> beta(h.N, vector<double>(sequence_size, 0.0));//matrica u  koju pohranjujem "unazadne" vjerojatnosti sekvenci

    for (int i = 0; i < h.N; i++){
		beta[i][sequence_size - 1] = 0;
	}
    

   for (int t = sequence_size - 2; t >= 0; t--)
	{
		for (int i = 0; i < h.N; i++) // po svim stanjima
		{
			double logsum = -INFINITY;
			for (int j = 0; j < h.N; j++)
			{
				double temp = log_product(log(h.A[i][j]), log_product(beta[j][t + 1], log(h.E[j][h.symbol_to_index[s[t + 1]]]))) ;
				logsum = log_sum(temp, logsum);
			}
			beta[i][t] = logsum;
		}
	}
    return beta;
}

#endif