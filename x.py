import numpy as np
import multiprocessing
from tqdm import tqdm
from collections import Counter
import itertools

def read_fasta(file_path):
    sequences = []
    current_sequence = ""
    current_header = ""

    total_lines = sum(1 for line in open(file_path, 'r'))

    with open(file_path, 'r') as file:
        for line in tqdm(file, total=total_lines, desc="Reading FASTA file"):
            line = line.strip()
            if line.startswith('>'):
                if current_header:
                    sequences.append(current_sequence)
                current_header = line[1:]
                current_sequence = ""
            else:
                current_sequence += line

    if current_header:
        sequences.append(current_sequence)

    return sequences

def preprocess(lista_parova):
    rezultat = []

    for pair1, pair2 in tqdm(lista_parova):
        prvi = ''.join(a for a, b in zip(pair1, pair2) if a != '-' or b != '-')
        drugi = ''.join(b for a, b in zip(pair1, pair2) if a != '-' or b != '-')
       
        
        novi_prvi = ""
        novi_drugi = ""

        
        for i, znak in enumerate(prvi):
            if (znak == '-' and i > 0 and drugi[i - 1] == '-') or (znak == '-' and i < (len(drugi) - 1) and drugi[i + 1] == '-'):
                continue
            novi_prvi += znak
            
        for i, znak in enumerate(drugi):
            if (znak == '-' and i > 0 and prvi[i - 1] == '-') or (znak == '-' and i < (len(prvi) - 1) and prvi[i + 1] == '-'):
                continue
            novi_drugi += znak

        rezultat.append((novi_prvi, novi_drugi))

    return rezultat


if __name__ == "__main__":
    multiprocessing.freeze_support()

    fasta_file_path = 'HIV1_ALL_2021_genome_DNA.fasta'
    sequences = read_fasta(fasta_file_path)[:500]
    print(f"Total number of sequences: {len(sequences)}")
    print(f"Length of sequences: {Counter([len(i) for i in sequences])}")
    pairs = list(itertools.permutations(sequences, 2))

    sequence_length = len(pairs[0][0])

    processed_pairs = preprocess(pairs)

    stanja = {}
    stanja_lista = []
    for pair in tqdm(processed_pairs):
        new_pair = [
        'M' if a != '-' and b != '-' else
        'Ix' if a != '-' and b == '-' else
        'Iy'
            for a, b in zip(pair[0], pair[1])
        ]
    
        stanja[pair] = new_pair
        stanja_lista.append(new_pair)




