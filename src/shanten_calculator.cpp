#include <algorithm>
#include <cstdint>
#include <format>
#include <mahjong/experimental/shanten_calculator.hpp>
#include <stdexcept>

namespace mahjong::experimental {
  constexpr int8_t MAX_SHT = 14;

  void chmin(int8_t& x, const int8_t& y)
  {
    if (x > y) x = y;
  }

  int8_t get_next_value(const int8_t& current, const int distance)
  {
    return current + std::max(distance, 0);
  }

  namespace standard {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                     const std::array<std::vector<Delta>, NUM_TIDS>& deltas,
                     const int m)
    {
      using std::array;

      array<array<array<array<array<int8_t, 5>, 2>, 5>, 5>, NUM_TIDS + 1> table;

      std::fill(table[0][0][0][0].begin(), table[NUM_TIDS][4][4][1].end(), MAX_SHT);
      table[0][0][0][0][0] = 0;

      for (int n = 0; n < NUM_TIDS; ++n) {
        for (int a = 0; a <= tile_limits[n]; ++a) {
          for (int b = 0; b <= tile_limits[n + 1]; ++b) {
            for (int h = 0; h <= 1; ++h) {
              for (int mm = 0; mm <= m; ++mm) {
                auto& current = table[n][a][b][h][mm];

                if (current == MAX_SHT) continue;

                for (const auto& delta : deltas[n]) {
                  if (a + delta.a > tile_limits[n] ||
                      b + delta.b > tile_limits[n + 1] ||
                      h + delta.h > 1 ||
                      mm + delta.m > 4) continue;

                  const int distance = a + delta.a - hand[n];

                  chmin(table[n + 1][b + delta.b][delta.c][h + delta.h][mm + delta.m],
                        get_next_value(current, distance));
                }
              }
            }
          }
        }
      }

      return table[NUM_TIDS][0][0][1][m] - 1;
    }
  }

  namespace seven_pairs {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits)
    {
      std::array<std::array<int8_t, 8>, NUM_TIDS + 1> table;

      std::fill(table[0].begin(), table[NUM_TIDS].end(), MAX_SHT);
      table[0][0] = 0;

      for (int n = 0; n < NUM_TIDS; ++n) {
        for (int p = 0; p <= 7; ++p) {
          auto& current = table[n][p];

          if (current == MAX_SHT) continue;

          for (int pp = 0; pp <= 1; ++pp) {
            if (2 * pp > tile_limits[n] || p + pp > 7) break;

            const int distance = std::max(2 * pp - hand[n], 0);

            chmin(table[n + 1][p + pp], get_next_value(current, distance));
          }
        }
      }

      return table[NUM_TIDS][7] - 1;
    }
  }

  namespace thirteen_orphans {
    int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                     const std::array<int, NUM_TIDS + 1>& tile_limits)
    {
      static constexpr std::array<int, 13> t = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
      std::array<std::array<int8_t, 2>, 14> table;

      std::fill(table[0].begin(), table[13].end(), MAX_SHT);
      table[0][0] = 0;

      for (int n = 0; n < 13; ++n) {
        for (int p = 0; p <= 1; ++p) {
          auto& current = table[n][p];

          if (current == MAX_SHT) continue;

          for (int pp = 0; pp <= 1; ++pp) {
            if (pp + 1 > tile_limits[t[n]] || p + pp > 1) break;

            const int distance = std::max(pp + 1 - hand[t[n]], 0);

            chmin(table[n + 1][p + pp], get_next_value(current, distance));
          }
        }
      }

      return table[13][1] - 1;
    }
  }

  int calc_shanten(const std::array<int, NUM_TIDS>& hand,
                   const std::array<int, NUM_TIDS + 1>& tile_limits,
                   const std::array<std::vector<standard::Delta>, NUM_TIDS>& deltas,
                   const int m,
                   const bool check_hand)
  {
    if (check_hand) {
      for (int i = 0; i < NUM_TIDS; ++i) {
        if (hand[i] < 0 || hand[i] > 4) {
          throw std::invalid_argument(std::format("Invalid number of hand's tiles at {}: {}", i, hand[i]));
        }
      }

      for (int i = 0; i < NUM_TIDS; ++i) {
        if (tile_limits[i] < 0 || tile_limits[i] > 4 || hand[i] > tile_limits[i]) {
          throw std::invalid_argument(std::format("Invalid number of tile_limits' at {}: {}", i, tile_limits[i]));
        }
      }

      if (m < 0 || m > 4) {
        throw std::invalid_argument(std::format("Invalid sum of hands's melds: {}", m));
      }
    }

    auto ret = MAX_SHT;

    {
      const auto sht = standard::calc_shanten(hand, tile_limits, deltas, m);

      chmin(ret, sht);
    }

    if (m == 4) {
      const auto sht = seven_pairs::calc_shanten(hand, tile_limits);

      chmin(ret, sht);
    }

    if (m == 4) {
      const auto sht = thirteen_orphans::calc_shanten(hand, tile_limits);

      chmin(ret, sht);
    }

    return ret;
  }

  std::array<int, NUM_TIDS + 1> make_tile_limits(const bool three_player)
  {
    decltype(make_tile_limits()) tile_limits;

    tile_limits.fill(4);

    if (three_player) {
      std::fill(&tile_limits[1], &tile_limits[8], 0);
    }

    return tile_limits;
  }

  std::array<std::vector<standard::Delta>, NUM_TIDS> make_deltas()
  {
    static const std::vector<standard::Delta> deltas_with_seq = {
        {0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1},
        {2, 2, 2, 0, 2},
        {3, 0, 0, 0, 1},
        {4, 1, 1, 0, 2},
        {2, 0, 0, 1, 0},
        {3, 1, 1, 1, 1},
        {4, 2, 2, 1, 2},
    };

    static const std::vector<standard::Delta> deltas_without_seq = {
        {0, 0, 0, 0, 0},
        {3, 0, 0, 0, 1},
        {2, 0, 0, 1, 0},
    };

    return {
        deltas_with_seq,    // 1m
        deltas_with_seq,    // 2m
        deltas_with_seq,    // 3m
        deltas_with_seq,    // 4m
        deltas_with_seq,    // 5m
        deltas_with_seq,    // 6m
        deltas_with_seq,    // 7m
        deltas_without_seq, // 8m
        deltas_without_seq, // 9m
        deltas_with_seq,    // 1p
        deltas_with_seq,    // 2p
        deltas_with_seq,    // 3p
        deltas_with_seq,    // 4p
        deltas_with_seq,    // 5p
        deltas_with_seq,    // 6p
        deltas_with_seq,    // 7p
        deltas_without_seq, // 8p
        deltas_without_seq, // 9p
        deltas_with_seq,    // 1s
        deltas_with_seq,    // 2s
        deltas_with_seq,    // 3s
        deltas_with_seq,    // 4s
        deltas_with_seq,    // 5s
        deltas_with_seq,    // 6s
        deltas_with_seq,    // 7s
        deltas_without_seq, // 8s
        deltas_without_seq, // 9s
        deltas_without_seq, // 1z
        deltas_without_seq, // 2z
        deltas_without_seq, // 3z
        deltas_without_seq, // 4z
        deltas_without_seq, // 5z
        deltas_without_seq, // 6z
        deltas_without_seq, // 7z
    };
  }
}
