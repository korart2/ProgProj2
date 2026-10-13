#pragma once

#include <cstddef>
#include <vector>

/// @brief Dot product computed in parallel.
/// The data is split into k nearly equal parts; each part is processed by
/// std::inner_product in its own thread, and the partial sums are combined by
/// std::reduce.
/// @param a First sequence.
/// @param b Second sequence, the same size as a.
/// @param k Number of parts (threads); k = 0 is treated as 1, k > a.size() is allowed.
/// @return The dot product of a and b.
double parallel_dot_product(const std::vector<double>& a, const std::vector<double>& b, std::size_t k);
