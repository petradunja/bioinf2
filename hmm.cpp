#include <string>
#include <iostream>
#include "hmm.h"

using namespace std;

// todo:konstruktor koji nije preko datoteke nego HMM(states,symbols,A,E,pi)

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

			// vector<string> states;
			for (int i = 0; i < N; ++i){
				file >> token;
				states.push_back(token);
				state_to_index[states[i]] = i;
			}
		} else if (key == "SIMBOLI") {

			// vector<string> symbols;

			for (int i = 0; i < M; ++i)
			{
				file >> token;
				symbols.push_back(token);
				symbol_to_index[symbols[i]] = i;
			}
		} else if (key == "A") {

			// A = matrix<double>(0, N, N);
			// vector<vector<double>> A(N, vector<double>(N, 0.0));
			// cout << A.size() << endl;
			// cout << N << endl;
			A.resize(N, vector<double>(N));

			// cout << A.size();
			for (int i = 0; i < N; ++i){
				for (int j = 0; j < N; ++j)
				{
					double value;
					file >> value;
					A[i][j] = value;
					// cout << A[i][j];
				}
			}
		} else if (key == "E") {
			// cout << "E" << endl;
			// E = matrix<double>(0, N, M);
			// vector<vector<double>> E(N, vector<double>(M, 0.0));
			E.resize(N, vector<double>(M));
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < M; ++j) {
					double value;
					file >> value;
					E[i][j] = value;
					// cout << E[i][j] << endl;
				}
			}
				
		} else if (key == "pi") {
			// cout << "Pi" << endl;
			// pi = new double[N];
			pi.resize(N);
			for (int i = 0; i < N; ++i){
				double value;
				file >> value;
				pi[i] = value;
				// cout << pi[i] << endl;
			}
		}
	}

	file.close();
}

void HMM::print()
{
	cout << "M (broj simbola) = " << M << endl;
	cout << "N (broj stanja)= " << N << endl;

	cout << "SIMBOLI:" << endl;
	for (const auto &symbol : symbols){
		cout << symbol << " ";
	}
	cout << endl;

	cout << "STANJA:" << endl;
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