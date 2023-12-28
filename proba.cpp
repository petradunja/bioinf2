#include "hmm.h"
#include "forward.h"
#include "backward.h"
#include "baumwelch.h"
#include "viterbi.h"

#include <iostream>
#include <unordered_map>
#include <string>

using std::cout;
using std::endl;

int main()
{
	

	// std::unordered_map<std::string, int> state_to_index;  // mapiranje stanja na indekse u matricama
	// std::unordered_map<std::string, int> symbol_to_index; // mapiranje parova simbola na indekse u matrici emisije

	// state_to_index["AA"] = 0;
    // state_to_index["AC"] = 1;
    // state_to_index["AG"] = 2;
	// state_to_index["AT"] = 3;
    // state_to_index["CA"] = 4;
    // state_to_index["CC"] = 5;
	// state_to_index["CG"] = 6;
    // state_to_index["CT"] = 7;
    // state_to_index["GA"] = 8;
	// state_to_index["GC"] = 9;
    // state_to_index["GG"] = 10;
    // state_to_index["GT"] = 11;
	// state_to_index["TA"] = 12;
    // state_to_index["TC"] = 13;
    // state_to_index["TG"] = 14;
	// state_to_index["TT"] = 15;
	// state_to_index["A"] = 16;
    // state_to_index["C"] = 17;
    // state_to_index["G"] = 18;
	// state_to_index["T"] = 19;


    // symbol_to_index["ix"] = 0;
    // symbol_to_index["iy"] = 1;
	// symbol_to_index["m"] = 2;


	std::string filename = "model.txt"; //inicijalno procjenjeni parametri na temelju HIV sekvenci
	HMM hmm(filename);
	
	hmm.print();

	//std::cout << hmm.E[1][1]<< std::endl; //ovo ne radi ako su parametri private
	//std::cout << hmm.pi[0] << std::endl;

	//TEST: POŠTENA/NEPOŠTENA KOCKA
	std::vector<std::string> s = {"5", "6", "2", "3", "6", "6"};
	std::string filename2 = "model2.txt"; 
	HMM hmm2(filename2);
	
	hmm2.print();


	//FORWARD ALGORITHM
	std::vector<std::vector<double>> alpha = forward(hmm2, s);
	//radi - provjereno u Ru
    for (const auto& row : alpha) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

	//BACKWARD ALGORITHM
	std::vector<std::vector<double>> beta = backward(hmm2, s);
	//radi - provjereno u Ru
    for (const auto& row : beta) {
        for (const auto& element : row) {
            std::cout << element << " " << "xx ";
        }
        std::cout << std::endl;
    }

	//BAUMWELCH ALGORITHM
	//radi - provjereno u Ru
	// HMM novi_model = baumWelch(hmm2,s,1,100);
	// std::cout << "kkk" << std::endl;
	// novi_model.print();

	// //VITERBI ALGORITHM
	// std::vector<std::string> path = viterbi(hmm2,s);
	// for (auto& state : path) {
    //         std::cout << state << " ";
    //     }
    



	return 0;
}