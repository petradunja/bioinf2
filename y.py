import multiprocessing
from tqdm import tqdm
import itertools
from collections import Counter

def read_fasta(file_path):
    sequences = []
    current_sequence = ""
    current_header = ""

    total_lines = sum(1 for line in open(file_path, 'r'))

    with open(file_path, 'r') as file:
        for line in tqdm(file, total=total_lines, desc="Reading FASTA file"):
            line = line.strip()
            if line.startswith('>'):
                if current_header: #dodaj sekvencu
                    sequences.append(current_sequence)
                current_header = line[1:]
                current_sequence = ""
            else:
                current_sequence += line

    #zadnja sekvenca
    if current_header:
        sequences.append(current_sequence)

    return sequences

def process_pair(pair):
    new_pair = (
        ''.join(a for a, b in zip(pair[0], pair[1]) if a != '-' or b != '-'),
        ''.join(b for a, b in zip(pair[0], pair[1]) if a != '-' or b != '-')
    )
    return new_pair

def parallel_process_pairs(pairs, num_processes=4):
    with multiprocessing.Pool(processes=num_processes) as pool:
        results = list(tqdm(pool.imap(process_pair, pairs), total=len(pairs), desc="Processing pairs"))

    return results

if __name__ == "__main__":
    # The following line is crucial for Windows support
    multiprocessing.freeze_support()

    # Example usage
    
    fasta_file_path = 'HIV1_ALL_2021_genome_DNA.fasta'
    sequences = read_fasta(fasta_file_path)[:1000]
    print(f"Ukupan broj sekvenci: {len(sequences)}")
    print(f"Duljina sekvenci: {Counter([len(i) for i in sequences])}")
    pairs = list(itertools.permutations(sequences, 2))
   
    duljina = len(pairs[0][0])  # sve sekvence su jednake duljine
    
    processed_pairs = parallel_process_pairs(pairs,num_processes = 8)
    

    print(processed_pairs)