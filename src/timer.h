#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <vector>

/// @brief Timing utilities for benchmarks.
namespace bench {

/// @brief Runs a callable several times and returns the median time in milliseconds.
/// The median is robust against occasional OS scheduling noise.
/// @tparam Func Callable returning double. The returned values are summed into a
/// volatile sink, so the compiler cannot discard the measured work.
/// @param func Code to measure.
/// @param repeats Number of runs.
/// @return Median run time in milliseconds.
template <typename Func>
double measure_median_ms(Func&& func, int repeats = 7) {
    std::vector<double> timings;
    timings.reserve(static_cast<std::size_t>(repeats));
    double checksum = 0.0;

    for (int i = 0; i < repeats; ++i) {
        const auto start = std::chrono::steady_clock::now();
        checksum += func();
        const auto end = std::chrono::steady_clock::now();
        timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }

    volatile double sink = checksum;
    (void)sink;

    std::ranges::sort(timings);
    return timings[timings.size() / 2];
}

}  // namespace bench
