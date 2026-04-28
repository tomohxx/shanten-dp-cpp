# Shanten DP CPP

A C++ implementation of the Shanten calculation algorithm using dynamic programming.

## Overview

This project provides an efficient algorithm to calculate the minimum number of tiles needed to form a tenpai hand in Mahjong, known as "Shanten" (向聴).

## Features

- Provably correct (理論的な正確性を保証)
- Fast DP-based computation (動的計画法ベースの高速計算)
- Supports arbitrary hand sizes (少牌/多牌に対応)
- Supports tile availability constraints (残り枚数の制約に対応)
  - Supports open melds (副露に対応)
  - Supports three-player mahjong (三人麻雀に対応)

## Requirements

- CMake 3.12 or later
- C++20 compatible compiler
- GoogleTest (optional, for tests)
- Google Benchmark (optional, for benchmarks)

## Build & Install

```bash
cd shanten-dp-cpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build build
cmake --install build
```

## Usage

For `calc_shanten(hand, tile_limits, deltas, m, check_hand)`, `m` is the number of meld blocks that still need to be formed.

### Examples

For examples, see [tests/shanten_calculator_test.cpp](tests/shanten_calculator_test.cpp).

### Benchmarks

1. Put Ara's test cases (`p_normal_10000.txt`, `p_hon_10000.txt`, `p_tin_10000.txt`, and `p_koku_10000.txt`) in [benches/data](benches/data)[^1].
1. Build `shanten_calculator_bench`.
1. Run `shanten_calculator_bench` with optional Google Benchmark flags such as `--benchmark_filter=normal`.

[^1]: Available from https://mahjong.ara.black/etc/shanten/shanten9.htm.

### Integration

Using the installed library from another CMake project:

```cmake
find_package(MahjongExperimental REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE MahjongExperimental::shanten_calculator)
```

## References

- [tomohxx/shanten-number](https://github.com/tomohxx/shanten-number)
