#ifndef MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP
#define MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP

#include <array>
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

  std::optional<int8_t> calc_shanten(const std::array<int, NUM_TIDS>& hand,
                                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                                     int m,
                                     unsigned int mode = 7u,
                                     bool check_hand = false);

  std::optional<Data> calc_shanten2(const std::array<int, NUM_TIDS>& hand,
                                    const std::array<int, NUM_TIDS + 1>& tile_limits,
                                    int m,
                                    unsigned int mode = 7u,
                                    bool check_hand = false);

  constexpr std::array<int, NUM_TIDS + 1> make_tile_limits(bool three_player = false)
  {
    std::array<int, NUM_TIDS + 1> tile_limits{};

    tile_limits.fill(4);

    if (three_player) {
      std::fill(tile_limits.begin() + 1, tile_limits.begin() + 8, 0);
    }

    return tile_limits;
  }
}

#endif
