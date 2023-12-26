#ifndef backward_H
#define backward_H

#include"hmm.h"
//vjerojatnost da je model u trenutku t u stanju i da će emitirati preostale simbole u trenutcim t+1 do T

std::vector<std::vector<double>> backward(HMM& h, std::vector<std::string>& s){ //funkcija prima model i sekvencu emitiranih parova simbola
    
    int sequence_size = int(s.size());
    std::vector<std::vector<double>> beta(h.N, std::vector<double>(sequence_size, 0.0));//matrica u  koju pohranjujem "unazadne" vjerojatnosti sekvenci

    for (int i = 0; i < h.N; i++){
		beta[i][sequence_size - 1] = 1;  //vjerojatnost da smo u trenutku T u stanju i 
	}
    

   

	for (int t = sequence_size - 2; t >= 0; t--)
	{
		for (int i = 0; i < h.N; i++) // po svim stanjima
		{
			double sum = 0;
			for (int j = 0; j < h.N; j++)
			{
				sum += beta[j][t + 1] * h.A[i][j] * h.E[j][h.symbol_to_index[s[t + 1]]] ;
			}
			beta[i][t] = sum;
		}
	}

	
	// double prob = 0; //vjerojatnost
	// for (int i = 0; i < h.N; ++i)
	// {
	// 	prob += h.pi[i] * h.E[i][h.symbol_to_index[s[0]]] * beta[i][0];
	// }



    return beta;
}

#endif