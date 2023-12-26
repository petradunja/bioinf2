
#ifndef BAUM_WELCH_H
#define BAUM_WELCH_H

#include"hmm.h"


// Baum-Welch algoritam za procjenu paramtera modela
//početni parametri su procjenjeni iz HIV sekvenci
// Expectation maximization algoritam - ponovna procjena parametara do konvergenciji ili maximalnog broja iteracija
//vraca se novi HMM model
HMM baumWelch(HMM& h, std::vector<std::string>& s, double delta, int num_iter); 

//zbroj razlike kvadrata po matricama - potrebno za odrediti je li algoritam konvergirao
double squaredDifference(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2);

#endif 
