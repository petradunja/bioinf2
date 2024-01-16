#include "hmm.h"
#include "baumwelch.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>


using namespace std;

vector<vector<string>> data;

void read_data(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return ;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> inner_vector;

        char ch;
        while (iss >> ch) {
            if (ch == '[') {
                string element;
                while (iss >> element && element.back() != ']') {
                  
                    // Remove leading and trailing whitespaces and the double quotes
                    element = element.substr(element.find_first_not_of(" \'"), element.find_last_not_of(" \'") -2);
                    inner_vector.push_back(element);
                }
                // Handle the last element in the vector
                if (!element.empty())
                    inner_vector.push_back(element);
            }
        }
        // Add the inner vector to the data vector
        data.push_back(inner_vector);
    }
}

int main()
{
    string model_filename = "datoteka.txt"; //inicijalno procjenjeni parametri na temelju HIV sekvenci
    HMM hmm(model_filename);
    cout << "-------HMM model--------" <<endl;
    hmm.print();
    cout << "-------END HMM model--------" << endl;

    string parovi_simbola_filename = "parovi.txt"; //parovi simbola
    read_data(parovi_simbola_filename);

    cout << "-------Baum Welch-------" << endl;
    int i = 1;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (const auto& data_vector : data) {
        vector<string> v = data_vector;
        cout << i << endl;
        HMM new_hmm = baumWelch(hmm, v, 0.1, 10, i++);
        hmm = new_hmm;
        hmm.print();
    }    
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    // Print the elapsed time
    std::cout << "-------------> Elapsed time: " << duration << " milliseconds" << std::endl;


    cout<<"BW" <<endl;
    hmm.print();

	/* // //VITERBI ALGORITHM
	cout<<"----------Viterbi----------" <<endl;
	vector<string> path = viterbi(hmm,s);
	cout<<"----------Viterbi states----------" <<endl;
	for (auto& state : path) {
      cout << state << " ";
   } */

	return 0;
}