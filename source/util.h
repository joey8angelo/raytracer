#pragma once

#include <cstdint>

inline float xorshift_0_1(const int &s1, const int &s2, const int &s3) {
  uint32_t x =
      0xDEADBEEF ^ (s1 * 0x12345678) ^ (s2 * 0x87654321) ^ (s3 * 0xABCDEF01);
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return static_cast<float>(x & 1) / 2.0 + 0.5; // Scale to [0, 1)
}
