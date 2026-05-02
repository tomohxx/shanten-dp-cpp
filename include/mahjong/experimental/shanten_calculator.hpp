#ifndef MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP
#define MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP

#include <array>

namespace mahjong::experimental {
  inline constexpr int NUM_TIDS = 34;

  namespace standard {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                     int m);
  }

  namespace seven_pairs {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits);
  }

  namespace thirteen_orphans {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits);
  }

  int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                   const std::array<int, NUM_TIDS + 1>& tile_limits,
                   int m,
                   bool check_hand = false);

  std::array<int, NUM_TIDS + 1> make_tile_limits(bool three_player = false);
}

#endif
