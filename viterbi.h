
#ifndef VITERBI_H
#define VITERBI_H

#include "hmm.h"


// Viterbijev algoritam vraća najvjerojatniji slijed skrivenih stanja; na temelju modela HMM i opservacije
std::vector<std::string> viterbi(HMM& h, std::vector<std::string>& s);

#endif 
