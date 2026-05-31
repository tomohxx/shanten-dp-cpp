#ifndef MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP
#define MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP

#include <array>
#include <optional>

namespace mahjong::experimental {
  inline constexpr int NUM_TIDS = 34;

  std::optional<int> calc_shanten(const std::array<int, NUM_TIDS>& hand,
                                  const std::array<int, NUM_TIDS + 1>& tile_limits,
                                  int m,
                                  unsigned int mode = 7u,
                                  bool check_hand = false);

  std::array<int, NUM_TIDS + 1> make_tile_limits(bool three_player = false);
}

#endif
