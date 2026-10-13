#pragma once

#include <cstddef>
#include <string>
#include <vector>

/// @brief Generation, saving and loading of the input sequences.
/// File format: the first line holds n, then n lines "a_i b_i" (two doubles
/// separated by a space).
namespace sequence_io {

/// @brief Two sequences of equal length.
struct SequencePair {
    std::vector<double> a;
    std::vector<double> b;
};

/// @brief Generates two sequences of n uniform doubles in [0, 1) and saves them to a file.
/// @param n Number of elements in each sequence.
/// @param filepath Output file; missing parent directories are created.
/// @param seed Seed of the random generator.
/// @throws std::runtime_error if the file cannot be opened.
void generate_and_save(std::size_t n, const std::string& filepath, unsigned int seed);

/// @brief Loads sequences saved by generate_and_save.
/// @param filepath Input file.
/// @return The loaded sequences.
/// @throws std::runtime_error if the file cannot be opened.
SequencePair load_sequence(const std::string& filepath);

/// @brief Builds the file path for a sequence of n elements.
/// @param n Number of elements.
/// @param directory Directory with the sequence files.
/// @return Path such as "data/sequence_1000000.txt".
std::string make_sequence_path(std::size_t n, const std::string& directory);

}  // namespace sequence_io
