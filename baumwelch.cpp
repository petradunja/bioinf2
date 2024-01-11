#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "baumwelch.h"
#include "forward.h"
#include "backward.h"
#include "hmm.h"

using namespace std;

double squaredDifference(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2) {
    double sum = 0.0;
    
    for (int i  = 0; i < matrix1.size(); ++i) {
        for (int j  = 0; j < matrix1[i].size(); ++j) {
            double diff = matrix1[i][j] - matrix2[i][j];
            sum += diff * diff; //suma kvadrata razlika po elementima matrice
        }
    }
    return sqrt(sum);
}


HMM baumWelch(HMM& h, vector<string>& s, double delta, int num_iter) {
    
    HMM hmm_temp = h; //u Ru tako definiraju, a ne mijenaju direktno parametre modela
	
    int T = s.size(); //vremenski trenutci; postoji T - 1 tranzicija
	 cout << "Vremenski trenutci: " << T << endl;

    for (int n = 0; n < num_iter; n++) {
        // Expectation step 
        vector<vector<double>> alpha = forward(hmm_temp,s);
        vector<vector<double>> beta = backward(hmm_temp,s);

        vector<vector<vector<double>>> xi(hmm_temp.N, vector<vector<double>>(hmm_temp.N, vector<double>(T - 1, 0.0))); //trodimenzionalna matrica ksi za praćenje tranzicija

        for (int t = 0; t < T - 1; t++){
		    double sum = -INFINITY;//sum[t]
		    for (int i = 0; i < hmm_temp.N; i++){
			    for (int j = 0; j < hmm_temp.N; j++){
					double temp = alpha[i][t] + log(hmm_temp.A[i][j]) + beta[j][t + 1] + log(hmm_temp.E[j][hmm_temp.symbol_to_index[s[t + 1]]]); //vjerojatnost da je u model u trenutku t u stanju i, u trenutku t+1 u stanju j s obzirom na emitiranu sekvencu i model H
					xi[i][j][t] = temp;
					if (temp > -INFINITY){
						sum = temp + log(1 + exp(sum-temp));
					}
			    }
		        }
			if (sum == - INFINITY){
				sum = 0;
			}
		    for (int i = 0; i < hmm_temp.N; i++){
			    for (int j = 0; j < hmm_temp.N; j++){
					xi[i][j][t] = xi[i][j][t] - sum; //marginalizacija
			    }
		    }
	    }
	
        // gamma - vjerojatnost da je model u trenutku t u stanju i s obzirom na emitiranu sekvencu i model H
        vector<vector<double>> gamma(hmm_temp.N, vector<double>(T, 0.0));
        for (int t = 0; t < T; t++){
		    double sum = -INFINITY;
		   for (int i = 0; i < hmm_temp.N; i++){
				double temp = alpha[i][t] + beta[i][t];
				gamma[i][t] = temp;
					if (temp > -INFINITY){
						sum = temp + log(1 + exp(sum-temp));
					}
         }
			if (sum != - INFINITY){
				for (int j = 0; j < hmm_temp.N; j++){
					gamma[j][t] = gamma[j][t] - sum;
				}
			}
			
        }        

        // Maximization step

        /* update A */
		for (int i = 0; i < hmm_temp.N; i++)
		{
			for (int j = 0; j < hmm_temp.N; j++)
			{
				double numerator = 0;
				double denominator = 0;
				for (int t = 0; t < T - 1; t++)
				{
					numerator += exp(xi[i][j][t]); //očekivani broj tranzicija iz stanja i u stanje j 
					denominator += exp(gamma[i][t]); //očekivani broj tranzcija iz stanja i 
				}
				hmm_temp.A[i][j] = numerator / denominator;
			}
		}

		/* update E */
		for (int i = 0; i < hmm_temp.N; i++) //stanja
		{
			for (int j = 0; j < hmm_temp.M; j++) //emitirani simboli
			{
				double numerator = 0;
				double denominator = 0;
				for (int t = 0; t < T; t++)
				{
					if (hmm_temp.symbol_to_index[s[t]] == j) //indikatorska funkcija provjerava je li emitiran simbol j
					{
						numerator += exp(gamma[i][t]); //očekivani broj emisija simbola j ustanju i 
					}
					denominator += exp(gamma[i][t]); //očekivani broj puta da je model u stanju i 
				}
				hmm_temp.E[i][j] = numerator / denominator;
			}
		}

		/* update pi */
		for (int i = 0; i < hmm_temp.N; ++i)
		{
			hmm_temp.pi[i] = exp(gamma[i][0]); //očekivani broj puta da je u trenutku t=0 model u stanju i 
		}

		cout << n << endl;
		hmm_temp.print();

        //provjera je li model konvergirao, prema kod u R-u
		double d  = squaredDifference(hmm_temp.A, h.A) +  squaredDifference(hmm_temp.E, h.E); //razlika u matricama prijelaza + razlika u matricama emisije
		
		 if (d < delta) {
            cout << "Breaking out of the loop at iteration " << n + 1 << " because d < delta." << endl;
            break;
        }

		

    }
	cout << "-------end-------" << endl;
	return hmm_temp; 
}


