#ifndef MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP
#define MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP

#include <array>
#include <concepts>
#include <cstdint>
#include <optional>

namespace mahjong::experimental {
  inline constexpr int NUM_TIDS = 34;

  struct Data {
    int8_t shanten;
    uint64_t discards;
    uint64_t waits;

    Data() = default;
    explicit Data(const int8_t shanten) : shanten(shanten), discards(0u), waits(0u) {}
    Data(const int8_t shanten, const uint64_t discards, const uint64_t waits)
        : shanten(shanten), discards(discards), waits(waits) {}
    bool operator==(const int8_t& rhs) const { return shanten == rhs; }
  };

  template <class T>
  concept Calculatable = std::same_as<T, int8_t> || std::same_as<T, Data>;

  template <Calculatable T>
  std::optional<T> calc_shanten(const std::array<int, NUM_TIDS>& hand,
                                const std::array<int, NUM_TIDS + 1>& tile_limits,
                                int m,
                                unsigned int mode = 7u,
                                bool check_hand = false);

  std::array<int, NUM_TIDS + 1> make_tile_limits(bool three_player = false);
}

#endif
