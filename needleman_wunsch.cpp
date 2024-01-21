/*
 Petar 'PetarV' Velickovic
 Algorithm: Needleman-Wunsch
*/

#include <chrono>


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <complex>
#include <fstream>
#include "poravnanje.h"

#define MAX_N 10001

#define DPRINTC(C) printf(#C " = %c\n", (C))
#define DPRINTS(S) printf(#S " = %s\n", (S))
#define DPRINTD(D) printf(#D " = %d\n", (D))
#define DPRINTLLD(LLD) printf(#LLD " = %lld\n", (LLD))
#define DPRINTLF(LF) printf(#LF " = %.5lf\n", (LF))

using namespace std;
typedef long long lld;
typedef unsigned long long llu;

int n, m;
int match_score, mismatch_score, gap_score;
string A, B;
int dp[MAX_N][MAX_N];

/*
 Needleman-Wunsch algorithm for determining the optimal alignment between two strings
 assuming a given score for hits, gaps and mismatches.
 Complexity: O(n * m) time, O(n * m) memory
*/

int needleman_wunsch(){
 

    //Inicijalizacija matrice
     for (int i = 0; i < MAX_N; ++i) {
        for (int j = 0; j < MAX_N; ++j) {
            dp[i][j] = 0;
            dp[i][j] = 0;
            dp[i][j] = 0;
        }
    }



    for (int i=0;i<=n;i++) dp[i][0] = dp[0][i] = -i * gap_score;
    for (int i=1;i<=n;i++)
    {
        //printf("%d\n",i);
        for (int j=1;j<=m;j++)
        {
            int S = (A[i-1] == B[j-1]) ? match_score : -mismatch_score;
            dp[i][j] = max(dp[i-1][j-1] + S, max(dp[i-1][j] - gap_score, dp[i][j-1] - gap_score));
        }
    }
    return dp[n][m];
}

inline pair<string, string> get_optimal_alignment()
{
    string retA, retB;
    stack<char> SA, SB;
    int ii = n, jj = m;
    while (ii != 0 || jj != 0)
    {
        if (ii == 0)
        {
            SA.push('-');
            SB.push(B[jj-1]);
            jj--;
        }
        else if (jj == 0)
        {
            SA.push(A[ii-1]);
            SB.push('-');
            ii--;
        }
        else
        {
            int S = (A[ii-1] == B[jj-1]) ? match_score : -mismatch_score;
            if (dp[ii][jj] == dp[ii-1][jj-1] + S)
            {
                SA.push(A[ii-1]);
                SB.push(B[jj-1]);
                ii--; jj--;
            }
            else if (dp[ii-1][jj] > dp[ii][jj-1])
            {
                SA.push(A[ii-1]);
                SB.push('-');
                ii--;
            }
            else
            {
                SA.push('-');
                SB.push(B[jj-1]);
                jj--;
            }
        }
    }
    while (!SA.empty())
    {
        retA += SA.top();
        retB += SB.top();
        SA.pop();
        SB.pop();
    }
    return make_pair(retA, retB);
}

int main()
{
   
   auto start_time = std::chrono::high_resolution_clock::now();

    match_score = 2, mismatch_score = 1, gap_score = 2;
    std::string file = "sekvence_za_poravnanje.txt";

  
    std::vector<std::pair<std::string, std::string>> parovi_sekvenci = readFile(file);

    //pohrana bodovanja poravnanja sekvenci
    std::vector<int> bodovi; 
    int i = 0;
    for (const auto &par : parovi_sekvenci) {
        
        A = par.first;
        B = par.second;
        
        n = A.length();
        m = B.length();
        
        printf("poravnato\n");
        printf("%d \n",i++);
        int bod = needleman_wunsch();
        bodovi.push_back(bod);
    }

    // Spremanje bodova u datoteku
    std::ofstream outFile("bodovanjeNW.txt");
    if (outFile.is_open()) {
        for (size_t i = 0; i < bodovi.size(); ++i) {
            outFile  << bodovi[i] << std::endl;
        }
        outFile.close();
        std::cout << "Bodovi su uspjesno zapisani" << std::endl;
    } else {
        std::cerr << "Greska" << std::endl;
    }

    // A = parovi_sekvenci[3].first;
    // B = parovi_sekvenci[3].second;
    // n = A.length();
    // m = B.length();
    // printf("%d %d\n",n, m);

    // printf("%d\n",needleman_wunsch());
    // printf("tu");
    // pair<string, string> alignment = get_optimal_alignment();
    // printf("tu2");
    // printf("%s\n%s\n", alignment.first.c_str(), alignment.second.c_str());

    // printf("%d \n %d \n", alignment.first.length(),alignment.second.length());
    // auto end_time = std::chrono::high_resolution_clock::now();
     
    // auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

   
    // std::cout << "Trajanje izvrsavanje programa: " << duration.count() << " sekundi." << std::endl;

    return 0;
}