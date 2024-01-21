#include <string>
#include <iostream>
#include "hmm.h"

using namespace std;



HMM::HMM(string &filename) {

	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Error: Unable to open file " << filename << endl;
		return;
	}

	string key;
	string token;
	while (file >> key){
		if (key == "M"){
			file >> M;
		} else if (key == "N") {
			file >> N;
		} else if (key == "STANJA") {

			
			for (int i = 0; i < N; ++i){
				file >> token;
				states.push_back(token);
				state_to_index[states[i]] = i;
			}
		} else if (key == "SIMBOLI") {

			

			for (int i = 0; i < M; ++i)
			{
				file >> token;
				symbols.push_back(token);
				symbol_to_index[symbols[i]] = i;
			}
		} else if (key == "A") {

		
			A.resize(N, vector<double>(N));

		
			for (int i = 0; i < N; ++i){
				for (int j = 0; j < N; ++j)
				{
					double value;
					file >> value;
					A[i][j] = value;
					
				}
			}
		} else if (key == "E") {
			
			E.resize(N, vector<double>(M));
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < M; ++j) {
					double value;
					file >> value;
					E[i][j] = value;
					
				}
			}
				
		} else if (key == "pi") {
		
			pi.resize(N);
			for (int i = 0; i < N; ++i){
				double value;
				file >> value;
				pi[i] = value;
			
			}
		}
	}

	file.close();
}

void HMM::print()
{
	cout << "M (broj simbola) = " << M << endl;
	cout << "N (broj stanja) = " << N << endl;

	cout << "SIMBOLI" << endl;
	for (const auto &symbol : symbols){
		cout << symbol << " ";
	}
	cout << endl;

	cout << "STANJA" << endl;
	for (const auto &state : states) {
		cout << state << " ";
	}
	cout << endl;

	cout << "MATRICA A: " << endl;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	cout << "MATRICA E: " << endl;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			cout << E[i][j] << " ";
		}
		cout << endl;
	}

	cout << "MATRICA pi: " << endl;
	for (int i = 0; i < N; ++i) {
		cout << pi[i] << " ";
	}
	cout << endl;

	// cout << "State to Index:" << endl;
	// for (const auto& pair : state_to_index) {
	//     cout << pair.first << " -> " << pair.second << endl;
	// }

	// cout << "Symbol to Index:" << endl;
	// for (const auto& pair : symbol_to_index) {
	//     cout << pair.first << " -> " << pair.second << endl;
	// }
}