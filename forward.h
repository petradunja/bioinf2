
#ifndef forward_H
#define forward_H

#include <iostream>
#include"hmm.h"

//možda bi trebalo raditi s logaritmima

std::vector<std::vector<double>> forward(HMM& h, std::vector<std::string>& s){ //funkcija prima model i sekvencu emitiranih parova simbola
    
    int sequence_size = int(s.size());
    std::vector<std::vector<double>> alpha(h.N, std::vector<double>(sequence_size, 0.0));//matrica u  koju pohranjujem "unaprijedne" vjerojatnosti sekvenci
    


    for (int i = 0; i < h.N; i++)
    {
        alpha[i][0] = h.pi[i] * h.E[i][h.symbol_to_index[s[0]]]; //vjerojatnost da je u trenutku t=0 u stanju i ako je emitiran par simbola s[0]
    }

    for (int t = 1; t < sequence_size; t++) //vremenski trenuci
    {
        for (int i = 0; i < h.N; i++) //stanja
        {
            double sum = 0;
            for (int j = 0; j < h.N; j++)
            {
                sum += alpha[j][t-1] * h.A[j][i]; //vjerojatnost da je u prošlom trenutku model bio u stanju j i da je prešao u stanje i
            }
            alpha[i][t] = sum * h.E[i][h.symbol_to_index[s[t]]]; //množenje s vjerojatnošću emisije simbola u trenutku t
        
    }
    }

    double prob = 0; //vjerojatnost promatrane sekvence
	for (int i = 0; i < h.N; ++i)
	{
		prob += alpha[i][sequence_size - 1]; //zbroji vjerojatnosti svakog stanja u posljednjem trenutku
	}

    return alpha;
}

#endif

