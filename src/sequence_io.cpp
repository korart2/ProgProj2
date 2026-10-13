#include "sequence_io.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <stdexcept>

namespace sequence_io {

void generate_and_save(std::size_t n, const std::string& filepath, unsigned int seed) {
    const std::filesystem::path path(filepath);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream out(filepath);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filepath);
    }

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    out << n << '\n' << std::setprecision(7);
    for (std::size_t i = 0; i < n; ++i) {
        const double x = dist(rng);
        const double y = dist(rng);
        out << x << ' ' << y << '\n';
    }
}

SequencePair load_sequence(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in) {
        throw std::runtime_error("Cannot open file for reading: " + filepath);
    }

    std::size_t n = 0;
    in >> n;

    SequencePair result;
    result.a.resize(n);
    result.b.resize(n);
    for (std::size_t i = 0; i < n; ++i) {
        in >> result.a[i] >> result.b[i];
    }
    return result;
}

std::string make_sequence_path(std::size_t n, const std::string& directory) {
    return directory + "/sequence_" + std::to_string(n) + ".txt";
}

}  // namespace sequence_io
