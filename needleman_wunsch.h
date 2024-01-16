
#ifndef NW_H
#define NW_H

#include <utility>
#include <string>
using namespace std;


// Needleman Wunsch algoritam - algoritam koji se koristi za globalno poravnanje dvije sekvence, dinamičko programiranje
int needleman_wunsch();
pair<string, string> get_optimal_alignment();

#endif 
