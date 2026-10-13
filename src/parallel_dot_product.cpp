#include "parallel_dot_product.h"

#include <algorithm>
#include <numeric>
#include <span>
#include <thread>

double parallel_dot_product(const std::vector<double>& a, const std::vector<double>& b, std::size_t k) {
    const std::size_t parts = std::max<std::size_t>(k, 1);
    const std::size_t base_size = a.size() / parts;
    const std::size_t remainder = a.size() % parts;

    std::vector<double> partial_sums(parts, 0.0);
    std::vector<std::thread> threads;
    threads.reserve(parts);

    std::size_t start = 0;
    for (std::size_t i = 0; i < parts; ++i) {
        // The first `remainder` parts get one extra element, so sizes differ by at most 1.
        const std::size_t size = base_size + (i < remainder ? 1 : 0);
        const std::span<const double> a_part = std::span(a).subspan(start, size);
        const std::span<const double> b_part = std::span(b).subspan(start, size);

        threads.emplace_back([a_part, b_part, &sum = partial_sums[i]] {
            sum = std::inner_product(a_part.begin(), a_part.end(), b_part.begin(), 0.0);
        });
        start += size;
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
    return std::reduce(partial_sums.begin(), partial_sums.end(), 0.0);
}
