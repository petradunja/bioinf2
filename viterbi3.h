#ifndef VITERBI3_H
#define VITERBI3_H

#include "hmm.h"

//korištenje Viterbijeva algoritma za poravananje sekvenci x i y, uz parametre odgovarajućeg HMM modela
std::pair<std::string, std::string> viterbi3(const std::string& x, const std::string& y,HMM& h);

//bodovanje dobivenog poravanja kao u NW algoritmu
int bodovanje (const std::string& x, const std::string& y);

#endif 
