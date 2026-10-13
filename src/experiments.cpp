#include "experiments.h"

#include <algorithm>
#include <cmath>
#include <execution>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <utility>

#include "parallel_dot_product.h"
#include "sequence_io.h"
#include "timer.h"

namespace experiments {
namespace {

using sequence_io::SequencePair;

constexpr int REPEATS = 7;
constexpr int K_REPEATS = 15;
constexpr double NEAR_BEST_FACTOR = 1.05;
constexpr double MAX_RELATIVE_DIFFERENCE = 1e-9;

struct KTiming {
    std::size_t k;
    double time_ms;
};

SequencePair load(std::size_t n, const std::string& data_dir) {
    return sequence_io::load_sequence(sequence_io::make_sequence_path(n, data_dir));
}

double dot_inner_product(const SequencePair& data) {
    return std::inner_product(data.a.begin(), data.a.end(), data.b.begin(), 0.0);
}

template <typename Policy>
double dot_transform_reduce(const Policy& policy, const SequencePair& data) {
    return std::transform_reduce(policy, data.a.begin(), data.a.end(), data.b.begin(), 0.0);
}

void print_k_table(const std::vector<KTiming>& timings, double best_time) {
    std::cout << std::left << std::setw(8) << "K" << std::setw(14) << "Time (ms)" << "Relative to best\n";
    std::cout << "------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);
    for (const KTiming& timing : timings) {
        std::cout << std::setw(8) << timing.k << std::setw(14) << timing.time_ms
                  << timing.time_ms / best_time << '\n';
    }
}

}  // namespace

void run_correctness_check(std::size_t n, const std::string& data_dir) {
    const SequencePair data = load(n, data_dir);

    const std::vector<std::pair<std::string, double>> results = {
        {"inner_product", dot_inner_product(data)},
        {"transform_reduce (seq)", dot_transform_reduce(std::execution::seq, data)},
        {"transform_reduce (par)", dot_transform_reduce(std::execution::par, data)},
        {"transform_reduce (par_unseq)", dot_transform_reduce(std::execution::par_unseq, data)},
        {"parallel_dot_product K=4", parallel_dot_product(data.a, data.b, 4)},
        {"parallel_dot_product K=9", parallel_dot_product(data.a, data.b, 9)},
    };

    std::cout << "\n=== Correctness check (N = " << n << ") ===\n" << std::fixed << std::setprecision(6);
    double max_difference = 0.0;
    for (const auto& [name, value] : results) {
        std::cout << std::left << std::setw(32) << name << value << '\n';
        max_difference = std::max(max_difference, std::abs(value - results.front().second) / results.front().second);
    }
    std::cout << "Max relative difference: " << std::scientific << std::setprecision(2) << max_difference
              << (max_difference < MAX_RELATIVE_DIFFERENCE ? "  -> OK" : "  -> MISMATCH") << '\n';
}

void run_library_experiment(const std::vector<std::size_t>& sizes, const std::string& data_dir) {
    std::cout << "\n=== Experiments 1-2: library algorithms, time in ms (median of " << REPEATS << " runs) ===\n";
    std::cout << std::left << std::setw(12) << "" << std::setw(16) << "inner_product"
              << "transform_reduce with policy:\n";
    std::cout << std::setw(12) << "N" << std::setw(16) << "(no policy)" << std::setw(14) << "seq"
              << std::setw(14) << "par" << "par_unseq\n";
    std::cout << "------------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);

    for (std::size_t n : sizes) {
        const SequencePair data = load(n, data_dir);

        const auto time_policy = [&data](const auto& policy) {
            return bench::measure_median_ms([&] { return dot_transform_reduce(policy, data); }, REPEATS);
        };
        const double t_plain = bench::measure_median_ms([&] { return dot_inner_product(data); }, REPEATS);
        const double t_seq = time_policy(std::execution::seq);
        const double t_par = time_policy(std::execution::par);
        const double t_par_unseq = time_policy(std::execution::par_unseq);

        std::cout << std::setw(12) << n << std::setw(16) << t_plain << std::setw(14) << t_seq
                  << std::setw(14) << t_par << t_par_unseq << '\n';
    }
}

void run_k_experiment(std::size_t n, const std::string& data_dir, const std::vector<std::size_t>& k_values) {
    const SequencePair data = load(n, data_dir);

    std::vector<KTiming> timings;
    for (std::size_t k : k_values) {
        const double time_ms =
            bench::measure_median_ms([&] { return parallel_dot_product(data.a, data.b, k); }, K_REPEATS);
        timings.push_back({k, time_ms});
    }
    const KTiming best = *std::ranges::min_element(timings, {}, &KTiming::time_ms);

    std::cout << "\n=== Experiment 3: custom parallel dot product, N = " << n << " (median of " << K_REPEATS
              << " runs) ===\n";
    print_k_table(timings, best.time_ms);

    const unsigned int hardware_threads = std::thread::hardware_concurrency();
    std::cout << "\nBest K = " << best.k << " (time = " << best.time_ms << " ms)\n";
    std::cout << "Hardware concurrency (CPU threads) = " << hardware_threads << '\n';
    std::cout << "Ratio best_K / hardware_concurrency = " << std::setprecision(4)
              << static_cast<double>(best.k) / static_cast<double>(hardware_threads) << '\n';

    std::cout << "K with time within 5% of the best:";
    for (const KTiming& timing : timings) {
        if (timing.time_ms <= best.time_ms * NEAR_BEST_FACTOR) {
            std::cout << ' ' << timing.k;
        }
    }
    std::cout << '\n';
}

}  // namespace experiments
