#include <iostream>
#include <vector>


#include"baumwelch.h"
using namespace std;
#include <cmath>



double squaredDifference(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) {
    double sum = 0.0;
    
    
    for (int i  = 0; i < matrix1.size(); ++i) {
        for (int j  = 0; j < matrix1[i].size(); ++j) {
            double diff = matrix1[i][j] - matrix2[i][j];
            sum += diff * diff; //suma kvadrata razlika po elementima matrice
        }
    }

    return std::sqrt(sum);
}


HMM baumWelch(HMM& h, std::vector<std::string>& s, double delta, int num_iter) {
	cout << "bw" << endl;
    
    HMM hmm_temp = h; //u Ru tako definiraju, a ne mijenaju direktno parametre modela
	
    
    int T = s.size(); //vremenski trenutci; postoji T - 1 tranzicija
	cout << T << endl;

    for (int n = 0; n < num_iter; n++) {
        // Expectation step 
        vector<vector<double>> alpha = forward(hmm_temp,s);
        vector<vector<double>> beta = backward(hmm_temp,s);

		for (const auto& row : alpha) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
       }

        vector<vector<vector<double>>> xi(hmm_temp.N, vector<vector<double>>(hmm_temp.N, vector<double>(T - 1, 0.0))); //trodimenzionalna matrica ksi za praćenje tranzicija

        for (int t = 0; t < T - 1; t++){
			cout << "T = " << t << endl;
		    double sum = 0;
		    for (int i = 0; i < hmm_temp.N; i++){
			    for (int j = 0; j < hmm_temp.N; j++){
					
				    xi[i][j][t] = alpha[i][t] * hmm_temp.A[i][j] * beta[j][t + 1] * hmm_temp.E[j][hmm_temp.symbol_to_index[s[t + 1]]]; //vjerojatnost da je u model u trenutku t u stanju i, u trenutku t+1 u stanju j s obzirom na emitiranu sekvencu i model H
				    sum += xi[i][j][t];
			    }
		        }
		    for (int i = 0; i < hmm_temp.N; i++){
			    for (int j = 0; j < hmm_temp.N; j++){
				xi[i][j][t] /= sum; //marginalizacija
			    }
		    }
	    }
	
		std::cout << "ksi" << endl;
        // gamma - vjerojatnost da je model u trenutku t u stanju i s obzirom na emitiranu sekvencu i model H
        vector<vector<double>> gamma(hmm_temp.N, vector<double>(T, 0.0));
        for (int t = 0; t < T; t++){
		    double sum = 0;
		    for (int i = 0; i < hmm_temp.N; i++){
                gamma[i][t] = alpha[i][t] * beta[i][t];
                sum += gamma[i][t];
            }
            for (int j = 0; j < hmm_temp.N; j++){
                gamma[j][t] /= sum;
            }

        }
		std::cout << "gamma" << endl;
        

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
					numerator += xi[i][j][t]; //očekivani broj tranzicija iz stanja i u stanje j 
					denominator += gamma[i][t]; //očekivani broj tranzcija iz stanja i 
				}
				hmm_temp.A[i][j] = numerator / denominator;
			}
		}
		std::cout << "gamma2" << endl;

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
						numerator += gamma[i][t]; //očekivani broj emisija simbola j ustanju i 
					}
					denominator += gamma[i][t]; //očekivani broj puta da je model u stanju i 
				}
				hmm_temp.E[i][j] = numerator / denominator;
			}
		}

		/* update pi */
		for (int i = 0; i < hmm_temp.N; ++i)
		{
			hmm_temp.pi[i] = gamma[i][0]; //očekivani broj puta da je u trenutku t=0 model u stanju i 
		}

        //provjera je li model konvergirao, prema kod u R-u
		double d  = squaredDifference(hmm_temp.A, h.A) +  squaredDifference(hmm_temp.E, h.E); //razlika u matricama prijelaza + razlika u matricama emisije
		
		 if (d < delta) {
            std::cout << "Breaking out of the loop at iteration " << n + 1 << " because d < delta." << std::endl;
            break;
        }

    }
	return hmm_temp; 
}


