#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <algorithm>
#include <string>

#include <utility>

using namespace std;


//čitanje parova sekvenci
std::vector<std::pair<std::string, std::string>> readFile( std::string& nazivDatoteke) {
    std::vector<std::pair<std::string, std::string>> rezultat;

   
    std::ifstream datoteka(nazivDatoteke);
  
    std::string linija;
      while (std::getline(datoteka, linija)) {
        // Ukloni '[' i ']' iz linije
        linija.erase(std::remove(linija.begin(), linija.end(), '['), linija.end());
        linija.erase(std::remove(linija.begin(), linija.end(), ']'), linija.end());

        // Razdvoji liniju na sekvenca1 i sekvenca2
        std::istringstream ss(linija);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            token.erase(std::remove(token.begin(), token.end(), ','), token.end());
            tokens.push_back(token);
        }

        // Ako postoje dva tokena, dodaj ih u vektor parova
        if (tokens.size() == 2) {
            rezultat.emplace_back(tokens[0], tokens[1]);
        }
    }
    

    datoteka.close();

    return rezultat;
}



