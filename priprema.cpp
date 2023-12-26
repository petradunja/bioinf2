


#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <thread>
#include <tuple>
#include <map>
#include <omp.h>

struct Counter {
    int operator[](std::size_t length) {
        return lengths[length]++;
    }

    friend std::ostream &operator<<(std::ostream &os, const Counter &counter) {
        for (auto it = counter.lengths.begin(); it != counter.lengths.end(); ++it) {
            os << "Length " << it->first << ": " << it->second << " sequences\n";
        }
        return os;
    }

private:
    std::map<int, int> lengths;
};

std::tuple<std::string, std::string> processPair(const std::string& seq1, const std::string& seq2) {
    std::string newSeq1, newSeq2;

    for (size_t i = 0; i < seq1.size(); ++i) {
        if (seq1[i] != '-' || seq2[i] != '-') {
            newSeq1 += seq1[i];
            newSeq2 += seq2[i];
        }
    }

    return std::make_tuple(newSeq1, newSeq2);
};





std::vector<std::string> read_fasta(const std::string &file_path) {
    std::vector<std::string> sequences;
    std::string current_sequence;
    std::string current_header;

    std::ifstream file(file_path);
    std::string line;

    int total_lines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');

    file.clear();
    file.seekg(0);

    // pohrana 500 sekvenci
    const int max_sequences = 500;
    int sequences_read = 0;

    
    while (std::getline(file, line) && sequences_read < max_sequences) {
        //std::getline(file, line);
        //line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        if (!line.empty() && line[0] == '>') { //pocetak nove sekvence
            if (!current_header.empty()) {
                sequences.push_back(current_sequence);
                ++sequences_read;
            }
            current_header = line.substr(1);
            current_sequence = "";
        } else {
            current_sequence += line;
        }
    }

    //if (!current_header.empty()) {
    //    sequences.push_back(current_sequence);
    //}


    return sequences;
};



int main() {

    std::string fasta_file_path = "HIV1_ALL_2021_genome_DNA.fasta";
    std::vector<std::string> sequences = read_fasta(fasta_file_path);

    

    std::cout << "Total number of sequences: " << sequences.size() << '\n';

    Counter counter;
    for (const auto &sequence : sequences) {
        counter[sequence.size()];
    }

    std::cout << "Lengths of sequences: " << counter;
    std::cout << "Prva sekvenca: " << sequences[0] << std::endl;

    try {
    // parovi sekvenci
    std::vector<std::pair<std::string, std::string>> sequence_pairs;
    for (size_t i = 0; i < 250 - 1; ++i) {
    for (size_t j = i + 1; j < 250; ++j) {
        // dodajemo i par (x, y) i par (y, x)
        sequence_pairs.push_back(std::make_pair(sequences[i], sequences[j]));
        sequence_pairs.push_back(std::make_pair(sequences[j], sequences[i]));
    }
}

    for (size_t i = 250; i < sequences.size() - 1; ++i) {
        for (size_t j = i + 1; j < sequences.size(); ++j) {
        // dodajemo i par (x, y) i par (y, x)
        sequence_pairs.push_back(std::make_pair(sequences[i], sequences[j]));
        sequence_pairs.push_back(std::make_pair(sequences[j], sequences[i]));
    }
    }


    std::cout << "Broj parova sekvenci: " << sequence_pairs.size() << std::endl;
    } catch (const std::bad_alloc& e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;

    }
  

    return 0;
}
