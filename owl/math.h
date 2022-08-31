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
    inline int mod(int a, int base) {
        return (a < 0 ? ((a % base) + base) % base : a % base);
    }

    template <typename type> inline std::vector<type> zeros(int d1) {
        return std::vector<type>(d1, 0);
    }

    template <typename type> inline std::vector<type> ones(int d1) {
        return std::vector<type>(d1, 1);
    }

    template <typename type> inline std::vector<std::vector<type>> zeros(int d1, int d2) {
        return std::vector<std::vector<type>>(d1, std::vector<type>(d2, 0));
    }

    template <typename type> inline std::vector<std::vector<type>> ones(int d1, int d2) {
        return std::vector<std::vector<type>>(d1, std::vector<type>(d2, 1));
    }

    template <typename type> inline std::vector<std::vector<std::vector<type>>> zeros(int d1, int d2, int d3) {
        return std::vector<std::vector<std::vector<type>>>(d1, std::vector<std::vector<type>>(d2, std::vector<type>(d3, 0)));
    }

    template <typename type> inline std::vector<std::vector<std::vector<type>>> ones(int d1, int d2, int d3) {
        return std::vector<std::vector<std::vector<type>>>(d1, std::vector<std::vector<type>>(d2, std::vector<type>(d3, 1)));
    }

    template <typename type> inline std::vector<std::vector<std::vector<std::vector<type>>>> zeros(int d1, int d2, int d3, int d4) {
        return std::vector<std::vector<std::vector<std::vector<type>>>>(d1, std::vector<std::vector<std::vector<type>>>(d2, std::vector<std::vector<type>>(d3, std::vector<type>(d4, 0))));
    }

    template <typename type> inline std::vector<std::vector<std::vector<std::vector<type>>>> ones(int d1, int d2, int d3, int d4) {
        return std::vector<std::vector<std::vector<std::vector<type>>>>(d1, std::vector<std::vector<std::vector<type>>>(d2, std::vector<std::vector<type>>(d3, std::vector<type>(d4, 1))));
    }

    template <typename type> inline std::vector<type> collect(int from, int to) {
        std::vector<type> v(to - from + 1);
        std::iota(v.begin(), v.end(), from);
        return v;
    }

    inline int double_to_int(double v) {
        return static_cast<int>(std::lround(v));
    }

    inline int floor(double v) {
        return static_cast<int>(std::floor(v));
    }

    inline bool approximately_equal(double a, double b, double rtol = 1e-4, double atol = 0) {
        //return std::abs(a - b) <= (epsilon * std::max(std::abs(a), std::abs(b)));
        return a == b || (std::isfinite(a) && std::isfinite(b) && std::abs(a - b) <= std::max(atol, rtol * std::max(std::abs(a), std::abs(b))));
    }

    inline bool essentially_equal(double a, double b, double rtol = 1e-4, double atol = 0) {
        //return std::abs(a - b) <= (epsilon * std::min(std::abs(a), std::abs(b)));
        return a == b || (std::isfinite(a) && std::isfinite(b) && std::abs(a - b) <= std::max(atol, rtol * std::min(std::abs(a), std::abs(b))));
    }

    inline bool definitely_greater_than(double a, double b, double epsilon = 1e-4) {
        return (a - b) > (epsilon * std::max(std::abs(a), std::abs(b)));
    }

    inline bool definitely_less_than(double a, double b, double epsilon = 1e-4) {
        return (b - a) > (epsilon * std::max(std::abs(a), std::abs(b)));
    }

    inline std::vector<std::pair<double, int>> vector_with_indices(std::vector<double>& v) {
        std::vector<std::pair<double, int>> pairs;
        for (std::size_t i = 0, size = v.size(); i < size; ++i) { pairs.push_back(std::make_pair(v[i], i)); }
        return pairs;
    }

    inline double round(double v, int digits) {
        return std::floor((v * std::pow(10.0, digits)) + .5) / std::pow(10.0, digits);
    }

    inline double significant_digits(double v, int digits) {
        if (v == 0.0) // otherwise it will return 'nan' due to the log10() of zero
            return 0.0;
        double factor = std::pow(10.0, digits - std::ceil(std::log10(std::fabs(v))));
        return std::round(v * factor) / factor;
    }

    inline std::pair<std::size_t, std::size_t> find_nearest(std::vector<double>& v, double t) {
        auto [min, max] = std::minmax_element(std::begin(v), std::end(v));

        auto lb = *min;
        std::size_t lb_index = min - std::begin(v);
        
        auto ub = *max;
        std::size_t ub_index = max - std::begin(v);

        for (std::size_t i = 0, size = v.size(); i < size; ++i) {
            auto value = v[i];

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