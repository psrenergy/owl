#pragma once

#ifndef OWL_MATH_H
#define OWL_MATH_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

namespace owl::math {
    inline int double_to_int(double value) {
        return (int)std::lround(value);
    }

    template <typename T> inline bool are_equal(double x, T y, double error = 1e-4) {
        return (std::abs)(x - (double)y) < error;
    }

    inline int nth_element(std::vector<double>& v, double percent) {
        auto size = v.size();
        if (size == 1) { return 0; }

        std::vector<std::pair<double, int>> numbers;
        for (int i = 0; i < size; ++i) { numbers.push_back(std::make_pair(v[i], i)); }

        auto shift = (std::min)((int)std::floor((percent * numbers.size()) / 100), (int)numbers.size() - 1);

        auto nth = numbers.begin() + shift;
        std::nth_element(numbers.begin(), nth, numbers.end());
        return (*nth).second;
    }

    inline double quantile(std::vector<double>& v, double percent) {
        if (v.size() == 0) return 0;

        auto nth = owl::math::nth_element(v, percent);
        return v[nth];
    }

    inline std::vector<double> get_cvar_weights(std::vector<double>& v, double alpha, bool inverted) {
        auto size = v.size();

        auto nth = owl::math::nth_element(v, alpha);
        auto pivot = v[nth];

        auto weight = 1.0 / (double)size;
        auto full_parts = (int)std::floor((alpha / 100.0) / weight);
        auto partial_weight = (alpha / 100.0) - (full_parts * weight);

        // SCALE TO 100%
        auto scale = 100.0 / alpha;
        weight *= scale;
        partial_weight *= scale;

        std::vector<double> weights;
        for (int i = 0; i < size; ++i) {
            auto value = v[i];

            if (value > pivot) {
                weights.push_back(inverted ? weight : 0);
            } else if (value < pivot) {
                weights.push_back(inverted ? 0 : weight);
            } else { // i == nth
                weights.push_back(partial_weight);
            }
        }

        return weights;
    }

    inline double cvar_left(std::vector<double>& v, double alpha) {
        auto weights = owl::math::get_cvar_weights(v, alpha, false);

        double sum = 0;
        auto size = v.size();

        for (int i = 0; i < size; ++i) {
            sum += weights[i] * v[i];
        }

        return sum;
    }

    inline double cvar_right(std::vector<double>& v, double alpha) {
        auto weights = owl::math::get_cvar_weights(v, alpha, true);

        double sum = 0;
        auto size = v.size();

        for (int i = 0; i < size; ++i) {
            sum += weights[i] * v[i];
        }
        return sum;
    }

    inline double stddev(std::vector<double>& v) {
        auto size = v.size();
        if (size == 1) {
            return 0;
        }

        double sum = std::accumulate(v.begin(), v.end(), 0.0);
        double mean = sum / size;

        std::vector<double> difference(v.size());
        std::transform(v.begin(), v.end(), difference.begin(), [mean](double x) { return x - mean; });

        double square_sum = std::inner_product(difference.begin(), difference.end(), difference.begin(), 0.0);
        return std::sqrt(square_sum / (size - 1));
    }

    inline double round(double v, int digits) {
        return std::floor((v * std::pow(10.0, digits)) + .5) / std::pow(10.0, digits);
    }
}

#endif