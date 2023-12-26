#include<string>
#include<iostream>


#include "hmm.h"

using std::string;
using std::cout;
using std::endl;

//todo:konstruktor koji nije preko datoteke nego HMM(states,symbols,A,E,pi)

HMM::HMM(std::string& filename)
{	

		std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::string key;
		std:string token;
        while (file >> key) {
            if (key == "M") {
				
                file >> M;
            } else if (key == "N") {
                file >> N;
				
			
            } else if (key == "STANJA"){
				
				//std::vector<std::string> states;
				for (int i = 0; i < N; ++i) {
					file >> token;
					states.push_back(token);
				
				
				
            		state_to_index[states[i]] = i;
        		}

			} else if (key == "SIMBOLI"){
				
				//std::vector<std::string> symbols;
				cout << "tu" << endl;
        		for (int i = 0; i < M; ++i) {
					file >> token;
					symbols.push_back(token);
            		symbol_to_index[symbols[i]] = i;
        		}
				cout << "tu3" << endl;

			}
			else if (key == "A") {
				
                //A = matrix<double>(0, N, N);
				//std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
				std::cout << A.size() << endl;
				std::cout << N << endl;
				A.resize(N, std::vector<double>(N));
				
				std::cout << A.size();
                for (int i = 0; i < N; ++i)
                    for (int j = 0; j < N; ++j){
						cout << i << j << endl;
						double value;
        				file >> value;
        				A[i][j] = value;
						cout << A[i][j];
						cout << "tuu";
					}

            } else if (key == "E") {
				cout << "E" << endl;
				//E = matrix<double>(0, N, M);
				//std::vector<std::vector<double>> E(N, std::vector<double>(M, 0.0));
				E.resize(N, std::vector<double>(M));
                for (int i = 0; i < N; ++i)
                    for (int j = 0; j < M; ++j){
						double value;
        				file >> value;
        				E[i][j] = value;
						cout << E[i][j] << endl;

						cout << "tuu";
					}
                        
            } else if (key == "pi") {
				cout << "Pi" << endl;
                //pi = new double[N];
				pi.resize(N);
                for (int i = 0; i < N; ++i){
					double value;
                    file >> value;
					pi[i] = value;
					cout << pi[i] << endl;

				}
            }
        }
		
        file.close();

        }

				




void HMM::print()
{
	cout << "M (broj simbola) = " << M << endl;
	cout << "N (broj stanja)= " << N << endl;

	std::cout << "SIMBOLI:" << std::endl;
        for (const auto& symbol : symbols) {
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
	
	std::cout << "STANJA:" << std::endl;
        for (const auto& state : states) {
            std::cout << state << " ";
        }
        std::cout << std::endl;
	

	cout << "MATRICA A: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	cout << "MATRICA E: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			cout << E[i][j] << " ";
		}
		cout << endl;
	}

	cout << "MATRICA pi: " << endl;
	for (int i = 0; i < N; ++i)
	{
		cout << pi[i] << " ";
	}
	cout << endl;

	// std::cout << "State to Index:" << std::endl;
    // for (const auto& pair : state_to_index) {
    //     std::cout << pair.first << " -> " << pair.second << std::endl;
    // }

    // std::cout << "Symbol to Index:" << std::endl;
    // for (const auto& pair : symbol_to_index) {
    //     std::cout << pair.first << " -> " << pair.second << std::endl;
    // }
}