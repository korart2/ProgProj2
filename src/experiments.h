#pragma once

#include <cstddef>
#include <string>
#include <vector>

/// @brief Experiments for variant 6: dot product of two sequences of doubles.
namespace experiments {

/// @brief Checks that all algorithms give the same dot product on the same data.
/// @param n Sequence size; the matching file must exist.
/// @param data_dir Directory with the sequence files.
void run_correctness_check(std::size_t n, const std::string& data_dir);

/// @brief Experiments 1 and 2: std::inner_product (no policy) and
/// std::transform_reduce with the seq, par and par_unseq policies.
/// @param sizes Sequence sizes; the matching files must exist.
/// @param data_dir Directory with the sequence files.
void run_library_experiment(const std::vector<std::size_t>& sizes, const std::string& data_dir);

/// @brief Experiment 3: time of the custom parallel algorithm for several K,
/// the best K and its ratio to the number of CPU threads.
/// @param n Sequence size; the matching file must exist.
/// @param data_dir Directory with the sequence files.
/// @param k_values Values of K (number of threads) to test.
void run_k_experiment(std::size_t n, const std::string& data_dir, const std::vector<std::size_t>& k_values);

}  // namespace experiments
