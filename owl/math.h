#pragma once

#ifndef OWL_MATH_H
#define OWL_MATH_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <limits>

namespace owl::math {
    inline int double_to_int(double value) {
        return (int)std::lround(value);
    }

    inline bool approximately_equal(double a, double b, double epsilon = 1e-4) {
        return std::abs(a - b) <= ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    inline bool essentially_equal(double a, double b, double epsilon = 1e-4) {
        return std::abs(a - b) <= ((std::abs(a) > std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    inline bool definitely_greater_than(double a, double b, double epsilon = 1e-4) {
        return (a - b) > ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
    }

    inline bool definitely_less_than(double a, double b, double epsilon = 1e-4) {
        return (b - a) > ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
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
        for (int i = 0, size = v.size(); i < size; ++i) { sum += weights[i] * v[i]; }
        return sum;
    }

    inline double cvar_right(std::vector<double>& v, double alpha) {
        auto weights = owl::math::get_cvar_weights(v, alpha, true);

        double sum = 0;
        for (int i = 0, size = v.size(); i < size; ++i) { sum += weights[i] * v[i]; }
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

    inline std::pair<int, int> find_nearest(std::vector<double>& v, int t) {
        auto [min, max] = std::minmax_element(std::begin(v), std::end(v));

        auto lb = *min;
        int lb_index = min - std::begin(v);
        
        auto ub = *max;
        int ub_index = max - std::begin(v);

        for (size_t i = 0, size = v.size(); i < size; ++i) {
            int value = v[i];

            if (lb < value && value <= t) {
                lb = value;
                lb_index = i;
            } 
            
            if (t <= value && value < ub) {
                ub = value;
                ub_index = i;
            }
        }

        return std::make_pair(lb_index, ub_index);
    }
}

#endif