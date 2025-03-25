#pragma once

#include <cstdint>

inline double xorshift_0_1(const int& s1, const int& s2, const int& s3) {
    uint8_t x = s1 ^ (s1 << 3);
    x ^= (x^s2) >> 7;
    x ^= (x^s3) << 5;

    return double(x)/(UINT8_MAX+55);
}