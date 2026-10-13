// Compiler used: MSVC 19.51.36260 (Visual Studio 2026 / Build Tools v18.10.3), Standard: C++23

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "experiments.h"
#include "sequence_io.h"

namespace {

const std::vector<std::size_t> SIZES = {100'000, 1'000'000, 5'000'000, 10'000'000, 20'000'000};
const std::vector<std::size_t> K_VALUES = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 24, 32, 64, 128, 256, 512, 1024};
constexpr unsigned int SEED = 42;
const std::string DATA_DIR = "data";

void print_usage() {
    std::cout << "Usage:\n"
                 "  ProgrammingProject2.exe --generate   Generate the input sequences into the data/ folder.\n"
                 "  ProgrammingProject2.exe --run        Run all experiments on the generated data.\n";
}

void generate_all_sequences() {
    std::cout << "Generating sequences into '" << DATA_DIR << "'...\n";
    for (std::size_t n : SIZES) {
        const std::string path = sequence_io::make_sequence_path(n, DATA_DIR);
        sequence_io::generate_and_save(n, path, SEED);
        std::cout << "  Generated " << path << " (" << n << " pairs)\n";
    }
    std::cout << "Done.\n";
}

void run_all_experiments() {
    experiments::run_correctness_check(SIZES.front(), DATA_DIR);
    experiments::run_library_experiment(SIZES, DATA_DIR);
    experiments::run_k_experiment(SIZES.back(), DATA_DIR, K_VALUES);
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string_view mode = argc == 2 ? argv[1] : "";
    try {
        if (mode == "--generate") {
            generate_all_sequences();
        } else if (mode == "--run") {
            run_all_experiments();
        } else {
            print_usage();
        }
    } catch (const std::exception& e) {
        std::cerr << "\n[Error] " << e.what() << " (run with --generate first?)\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
