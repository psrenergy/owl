#pragma once

#ifndef OWL_CONVERT_H
#define OWL_CONVERT_H

namespace owl::convert {
    inline constexpr double pi = 3.14159265358979323846;

    inline double mw_to_pu(double value) {
        return value / 100.0;
    }

    inline double pu_to_mw(double value) {
        return value * 100.0;
    }

    inline double degree_to_radian(double value) {
        return (value * pi) / 180;
    }

    inline double percent_to_pu(double value) {
        return value / 100;
    }
}

#endif