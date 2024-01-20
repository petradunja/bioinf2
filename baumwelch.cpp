#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "baumwelch.h"
#include "forward.h"
#include "backward.h"
#include "hmm.h"
#include "log_utils.h"

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


HMM baumWelch(HMM& h, vector<string>& s, double delta, int num_iter, int broj) {
    
   HMM hmm_temp = h; //u Ru tako definiraju, a ne mijenaju direktno parametre modela
	HMM hmm_stari= h;
	
   int T = s.size(); //vremenski trenutci; postoji T - 1 tranzicija
	std::cout << "Vremenski trenutci: " << T << endl;

    for (int n = 0; n < num_iter; n++) {
      // Expectation step 
      vector<vector<double>> alpha = forward(hmm_temp,s);
      vector<vector<double>> beta = backward(hmm_temp,s);

      vector<vector<vector<double>>> xi(hmm_temp.N, vector<vector<double>>(hmm_temp.N, vector<double>(T - 1, 0))); //trodimenzionalna matrica ksi za praćenje tranzicija

		for (int t = 0; t < T - 1; t++){
			double sumb = -INFINITY;
			for (int i = 0; i < hmm_temp.N; i++){
				for (int j = 0; j < hmm_temp.N; j++){
					xi[i][j][t] = log_product(alpha[i][t], 
										log_product(compute_log(hmm_temp.A[i][j]), 
										log_product(compute_log(hmm_temp.E[j][hmm_temp.symbol_to_index[s[t + 1]]]), beta[j][t + 1]))); //vjerojatnost da je u model u trenutku t u stanju i, u trenutku t+1 u stanju j s obzirom na emitiranu sekvencu i model H
					sumb = log_sum(sumb, xi[i][j][t]);
				}
			}

			for (int i = 0; i < hmm_temp.N; i++){
				for (int j = 0; j < hmm_temp.N; j++){
					xi[i][j][t] = log_product(xi[i][j][t], -sumb);
				}
			}
		}
	
		// gamma - vjerojatnost da je model u trenutku t u stanju i s obzirom na emitiranu sekvencu i model H
		vector<vector<double>> gamma(hmm_temp.N, vector<double>(T, 0));

		for (int t = 0; t < T; t++){
			double suma = -INFINITY;
			for (int i = 0; i < hmm_temp.N; i++){
				gamma[i][t] = log_product(alpha[i][t], beta[i][t]);
				suma = log_sum(suma, gamma[i][t]);
			}
			for (int i = 0; i < hmm_temp.N; i++){
				gamma[i][t] = log_product(gamma[i][t], -suma);
			}
		}        

        // Maximization step

        /* update A */
		for (int i = 0; i < hmm_temp.N; i++)
		{
			for (int j = 0; j < hmm_temp.N; j++)
			{
				double numerator = -INFINITY;
				double denominator = -INFINITY;
				for (int t = 0; t < T - 1; t++)
				{
					numerator = log_sum(numerator, xi[i][j][t]); //očekivani broj tranzicija iz stanja i u stanje j 
					denominator = log_sum(denominator, gamma[i][t]); //očekivani broj tranzcija iz stanja i 
				}
				hmm_temp.A[i][j] = compute_exp(log_product(numerator,-denominator));
			}

			for (int k = 0; k < hmm_temp.M; k++) //emitirani simboli
			{
				double numerator = -INFINITY;
				double denominator = -INFINITY;
				for (int t = 0; t < T; t++)
				{
					if (hmm_temp.symbol_to_index[s[t]] == k) //indikatorska funkcija provjerava je li emitiran simbol j
					{
						numerator = log_sum(numerator, gamma[i][t]); //očekivani broj emisija simbola j ustanju i 
					}
					denominator = log_sum(denominator, gamma[i][t]); //očekivani broj puta da je model u stanju i 
				}

				hmm_temp.E[i][k] = compute_exp(log_product(numerator, -denominator));
			}

			hmm_temp.pi[i] = compute_exp(gamma[i][0]);
		}

		
		
      //provjera je li model konvergirao, prema kod u R-u
		double d1  = squaredDifference(hmm_temp.A, hmm_stari.A);
		

		if (d1 < delta) {
			double d2 = squaredDifference(hmm_temp.E, hmm_stari.E); //razlika u matricama prijelaza + razlika u matricama emisije
			
			hmm_stari = hmm_temp;
			if ((d1+d2) < delta) {
				std::cout << "Breaking out of the loop at iteration " << n + 1 << " because d < delta." << endl;
				break;
			} 
		} 
		hmm_stari = hmm_temp;
   }
	//delta za viterbija
	double prosjek1 = (hmm_temp.A[0][1] + hmm_temp.A[0][2])/2;
	hmm_temp.A[0][1] = hmm_temp.A[0][2] = prosjek1;

	//epsilon
	double prosjek2 = (hmm_temp.A[1][1] + hmm_temp.A[2][2])/2;
	hmm_temp.A[1][1] = hmm_temp.A[2][2] = prosjek2;

	//1-epsilon
	hmm_temp.A[1][0] = 1 - prosjek2;
	hmm_temp.A[2][0] = 1- prosjek2;

	hmm_temp.A[1][2] = 1e-008;
	hmm_temp.A[2][1] = 1e-008;


	std::cout << "-------end-------" << endl;
	return hmm_temp; 
}


