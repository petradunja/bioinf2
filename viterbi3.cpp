#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

#include <chrono>


using namespace std;


#include "hmm.h"
#include "poravnanje.h"
#define MAX_N 10001

struct ViterbiInfo {
    float M;
    float Ix;
    float Iy;
    int backTrack;
};

struct ViterbiInfo2 {
     int backtrackDirection[3]; // 0 - M, 1 - Ix, 2 - Iy
};




ViterbiInfo2 matrix2[MAX_N][MAX_N];


pair<string, string> viterbi3(const std::string& x, const std::string& y,HMM& h){
    int len_x = x.length();
    int len_y = y.length();
    std::cout << len_x << len_y << std::endl;
    
    //Inicijalizacija matrice
     for (int i = 0; i < MAX_N; ++i) {
        for (int j = 0; j < MAX_N; ++j) {
            matrix2[i][j].backtrackDirection[0] = 0;
            matrix2[i][j].backtrackDirection[1] = 0;
            matrix2[i][j].backtrackDirection[2] = 0;
        }
    }



   

    // Inicijalizacija trenutne i prethodne matrice
    std::vector<ViterbiInfo> currentMatrix(len_y + 1, {  -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()});
    std::vector<ViterbiInfo> previousMatrix(len_y + 1, { -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() });

    previousMatrix[0].M = 0;



    printf("tu2");

    double delta = h.A[0][1]; //vjerojatnost prijelaza iz M u Ix
    double delta2 = h.A[0][2]; // vjerojatnost prijelaza iz Mu Iy
    double MuM = h.A[0][0]; // vjerojatnost ostanka M u M
    double epsilon = h.A[1][1]; //vjerojatnost prijelaza iz Ix u Ix; tj Iy u Iy
    double epsilon2 = h.A[2][2];

    double YuM = h.A[2][0]; // vjerojatnost prijelaza iz Iy u M
    double XuM = h.A[1][0]; //vjerojatnost prijelaza iz Ix u M

    for (int i = 1; i <= len_x; ++i) {
        if (i%100 == 0){
           printf("%d\n",i);
        }
  
        for (int j = 1; j <= len_y; ++j) {

          
            std::string pair = std::string(1, x[i-1]) + std::string(1, y[j-1]);

            

        // //     //odredivanje maksimuma za M

       
            std::array<double, 3> scores_M = {log(MuM) + previousMatrix[j-1].M, log(XuM) + previousMatrix[j-1].Ix, log(YuM) + previousMatrix[j-1].Iy};
            currentMatrix[j].M = log(h.E[0][h.symbol_to_index[pair]]) + std::max({log(MuM) + previousMatrix[j-1].M, log(XuM) + previousMatrix[j-1].Ix, log(YuM) + previousMatrix[j-1].Iy});

   
            // // //odredivanje maksimuma za Ix
       
         

            std::array<double, 2> scores_Ix = { log(delta) + previousMatrix[j].M, log(epsilon) + previousMatrix[j].Ix };
            currentMatrix[j].Ix = log(h.E[1][h.symbol_to_index[std::string(1, x[i-1])]]) + std::max({ log(delta) + previousMatrix[j].M, log(epsilon) + previousMatrix[j].Ix });

    
        //     //odredivanje maksimuma za Iy
    



           std::array<double, 2> scores_Iy = { log(delta2) + currentMatrix[j-1].M, log(epsilon2) + currentMatrix[j-1].Iy};
           currentMatrix[j].Iy = log(h.E[2][h.symbol_to_index[std::string(1, y[j-1])]]) + std::max({ log(delta2) + currentMatrix[j-1].M, log(epsilon2) + currentMatrix[j-1].Iy});

       
        
        

            //kako se doslo u pojedino stanje
            int maxIndex_M = std::distance(scores_M.begin(), std::max_element(scores_M.begin(), scores_M.end()));
            matrix2[i][j].backtrackDirection[0] = maxIndex_M;
          
                
            int maxIndex_Ix = std::distance(scores_Ix.begin(), std::max_element(scores_Ix.begin(), scores_Ix.end()));
            matrix2[i][j].backtrackDirection[1] = maxIndex_Ix;

                
            int maxIndex_Iy = std::distance(scores_Iy.begin(), std::max_element(scores_Iy.begin(), scores_Iy.end()));
            if (maxIndex_Iy == 1){
                    maxIndex_Iy = 2;
            }
                
            matrix2[i][j].backtrackDirection[2] = maxIndex_Iy;

            }
        // std::swap(currentMatrix, previousMatrix);
         previousMatrix = currentMatrix;
        
        }

    //backtracking
    int i = x.length();
    int j = y.length();
    std::string alignmentX, alignmentY;

    //std::array<double, 3> scores = {matrix[i][j].M, matrix[i][j].Ix, matrix[i][j].Iy};
    std::array<double, 3> scores = {previousMatrix[j].M, previousMatrix[j].Ix, previousMatrix[j].Iy};

    int maxIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
    //zadnje stanje je maxIndex
    
    while (i != 0 || j != 0) {

        if (i == 0)
        {
            alignmentX = '-' + alignmentX;
            alignmentY = y[j-1] + alignmentY;
            j--;
        }
        else if (j == 0)
        {
            alignmentX = x[i-1] + alignmentX;
            alignmentY = '-' + alignmentY;
            i--;
        }
        else {
            
       
        int direction = matrix2[i][j].backtrackDirection[maxIndex];//kako se došlo u zadnje stanje
        maxIndex = direction; // u iducoj iteraciji ovo ce biti prethodno stanje

        if (direction == 0) {
            // M
            alignmentX = x[i-1] + alignmentX;
            alignmentY = y[j-1] + alignmentY;
            i--;
            j--;
        } else if (direction == 1) {
            // Ix
            alignmentX = x[i-1] + alignmentX;
            alignmentY = '-' + alignmentY;
            i--;
        } else if (direction == 2) {
            // Iy
            alignmentX = '-' + alignmentX;
            alignmentY = y[j-1] + alignmentY;
            j--;
        }
        }
    }
    return make_pair(alignmentX,alignmentY);
}


int bodovanje (const std::string& x, const std::string& y){
     
    int bodovanje = 0;

    for (size_t i = 0; i < x.length(); ++i) {
        if (x[i] == '-' || y[i] == '-') {
            bodovanje += -2;
        } else if (x[i] == y[i]) {
            bodovanje += 2;
        } else {
            bodovanje += -1;
        }
    }
    return bodovanje;
}


int main() {
    std::string filename = "vani_0,01-100.txt"; //inicijalno procjenjeni parametri na temelju HIV sekvenci
	HMM hmm(filename);
    hmm.print();
    std::string file = "sekvence_za_poravnanje.txt";

  
    std::vector<std::pair<std::string, std::string>> parovi_sekvenci = readFile(file);
	
    //pohrana bodovanja poravnanja sekvenci
    std::vector<int> bodovi; 

    for (const auto &par : parovi_sekvenci) {
        
     std::pair<std::string, std::string> alignment = viterbi3(par.first,par.second,hmm);
        printf("poravnato\n");
        int bod = bodovanje(alignment.first,alignment.second);
        bodovi.push_back(bod);
    }

    // Spremanje bodova u datoteku
    std::ofstream outFile("bodovanjeViterbi.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < bodovi.size(); ++i) {
            outFile  << bodovi[i] << std::endl;
        }
        outFile.close();
        std::cout << "Bodovi su uspješno zapisani" << std::endl;
    } else {
        std::cerr << "Greška pri otvaranju datoteke bodovanja.txt." << std::endl;
    }


        auto start_time = std::chrono::high_resolution_clock::now();
        std::cout << "Sekvenca1: " << parovi_sekvenci[3].first << " Sekvenca2: " << parovi_sekvenci[3].second << std::endl;

  
        std::pair<std::string, std::string> alignment = viterbi3(parovi_sekvenci[3].first,parovi_sekvenci[3].second,hmm);
        
        printf("poravnato\n");
        printf("%s\n%s\n", alignment.first.c_str(), alignment.second.c_str());

        auto end_time = std::chrono::high_resolution_clock::now();
     
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

   
        std::cout << "Trajanje izvrsavanje programa: " << duration.count() << " sekundi." << std::endl;
   
        std::cout << bodovanje(alignment.first,alignment.second) << std::endl;

    return 0;
}
