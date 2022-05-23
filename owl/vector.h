#pragma once

#ifndef OWL_VECTOR_H
#define OWL_VECTOR_H

#include <algorithm>
#include <unordered_set>
#include <vector>

namespace owl::vector {
    template <typename type> inline void remove_duplicates(std::vector<type>& v) {
        std::unordered_set<type> seen;

        auto new_end = std::remove_if(v.begin(), v.end(), [&seen](const type& value) {
            if (seen.find(value) != std::end(seen)) {
                return true;
            }

            seen.insert(value);
            return false;
        });

        v.erase(new_end, v.end());
    }

    template <typename type> inline bool has(std::vector<type>& v, type item) {
        return std::find(v.begin(), v.end(), item) != v.end();
    }

    template <typename type> inline bool has(std::vector<type>& v, std::unordered_set<type>& items) {
        for (auto& item : items) {
            if (owl::vector::has(v, item)) return true;
        }
        return false;
    }
}

#endif