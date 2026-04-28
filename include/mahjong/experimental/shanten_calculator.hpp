#ifndef MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP
#define MAHJONG_EXPERIMENTAL_SHANTEN_CALCULATOR_HPP

#include <array>
#include <vector>

namespace mahjong::experimental {
  inline constexpr int NUM_TIDS = 34;

  namespace standard {
    struct Delta {
      int a;
      int b;
      int c;
      int h;
      int m;
    };

    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                     const std::array<std::vector<Delta>, NUM_TIDS>& deltas,
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
                   const std::array<std::vector<standard::Delta>, NUM_TIDS>& deltas,
                   int m,
                   bool check_hand = false);

  std::array<int, NUM_TIDS + 1> make_tile_limits(bool three_player = false);
  std::array<std::vector<standard::Delta>, NUM_TIDS> make_deltas();
}

#endif
