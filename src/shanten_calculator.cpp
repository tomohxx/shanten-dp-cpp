#include <algorithm>
#include <format>
#include <mahjong/experimental/shanten_calculator.hpp>
#include <stdexcept>
#include <vector>

namespace mahjong::experimental {
  constexpr int8_t MAX_SHT = 100;

  void chmin(int8_t& x, const int8_t& y)
  {
    if (x > y) x = y;
  }

  int8_t get_next_value(const int8_t& current, const int distance, const int)
  {
    return current + std::max(distance, 0);
  }

  void chmin(Data& x, const Data& y)
  {
    if (x.shanten > y.shanten) {
      x = y;
    }
    else if (x.shanten == y.shanten) {
      x.discards |= y.discards;
      x.waits |= y.waits;
    }
  }

  Data get_next_value(const Data& current, const int distance, const int n)
  {
    return {
        static_cast<int8_t>(current.shanten + std::max(distance, 0)),
        distance < 0 ? (current.discards | 1ull << n) : current.discards,
        distance > 0 ? (current.waits | 1ull << n) : current.waits,
    };
  }

  namespace standard {
    struct Delta {
      int a;
      int b;
      int c;
      int p;
      int m;
    };

    std::array<std::vector<Delta>, NUM_TIDS> make_deltas();

    template <class T>
    T calc_shanten(const std::array<int, NUM_TIDS>& hand,
                   const std::array<int, NUM_TIDS + 1>& tile_limits,
                   const int m)
    {
      static const auto deltas = make_deltas();

      using std::array;

      array<array<array<array<array<T, 5>, 2>, 5>, 5>, NUM_TIDS + 1> table;

      std::fill(table[0][0][0][0].begin(), table[NUM_TIDS][4][4][1].end(), T(MAX_SHT));
      table[0][0][0][0][0] = T(-1);

      for (int n = 0; n < NUM_TIDS; ++n) {
        for (const auto& delta : deltas[n]) {
          for (int a = 0; a <= tile_limits[n] - delta.a; ++a) {
            for (int b = 0; b <= std::min(tile_limits[n + 1] - delta.b, a); ++b) {
              for (int p = 0; p <= 1 - delta.p; ++p) {
                for (int mm = 0; mm <= m - delta.m; ++mm) {
                  const auto& current = table[n][a][b][p][mm];

                  if (current == MAX_SHT) continue;

                  const int distance = a + delta.a - hand[n];

                  chmin(table[n + 1][b + delta.b][delta.c][p + delta.p][mm + delta.m],
                        get_next_value(current, distance, n));
                }
              }
            }
          }
        }
      }

      return table[NUM_TIDS][0][0][1][m];
    }
  }

  namespace seven_pairs {
    template <class T, bool FourTileSevenPairs>
    T calc_shanten(const std::array<int, NUM_TIDS>& hand,
                   const std::array<int, NUM_TIDS + 1>& tile_limits)
    {
      std::array<std::array<T, 8>, NUM_TIDS + 1> table;

      std::fill(table[0].begin(), table[NUM_TIDS].end(), T(MAX_SHT));
      table[0][0] = T(-1);

      for (int n = 0; n < NUM_TIDS; ++n) {
        int pp_end;

        if constexpr (FourTileSevenPairs) {
          pp_end = tile_limits[n] / 2;
        }
        else {
          pp_end = std::min(tile_limits[n] / 2, 1);
        }

        for (int pp = 0; pp <= pp_end; ++pp) {
          for (int p = 0; p <= 7 - pp; ++p) {
            const auto& current = table[n][p];

            if (current == MAX_SHT) continue;

            const int distance = std::max(2 * pp - hand[n], 0);

            chmin(table[n + 1][p + pp], get_next_value(current, distance, n));
          }
        }
      }

      return table[NUM_TIDS][7];
    }
  }

  namespace thirteen_orphans {
    template <class T>
    T calc_shanten(const std::array<int, NUM_TIDS>& hand,
                   const std::array<int, NUM_TIDS + 1>& tile_limits)
    {
      static constexpr std::array<int, 13> t = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
      std::array<std::array<T, 2>, 14> table;

      std::fill(table[0].begin(), table[13].end(), T(MAX_SHT));
      table[0][0] = T(-1);

      for (int n = 0; n < 13; ++n) {
        for (int pp = 0; pp <= std::min(tile_limits[t[n]] - 1, 1); ++pp) {
          for (int p = 0; p <= 1 - pp; ++p) {
            const auto& current = table[n][p];

            if (current == MAX_SHT) continue;

            const int distance = std::max(pp + 1 - hand[t[n]], 0);

            chmin(table[n + 1][p + pp], get_next_value(current, distance, t[n]));
          }
        }
      }

      return table[t.size()][1];
    }
  }

  template <class T>
  std::optional<T> calc_shanten_impl(const std::array<int, NUM_TIDS>& hand,
                                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                                     const int m,
                                     unsigned int mode,
                                     const bool four_tile_seven_pairs,
                                     const bool check_hand)
  {
    if (check_hand) {
      for (int i = 0; i < NUM_TIDS; ++i) {
        if (hand[i] < 0 || hand[i] > 4) {
          throw std::invalid_argument(std::format("Invalid number of hand's tiles at {}: {}", i, hand[i]));
        }

        if (tile_limits[i] < 0 || tile_limits[i] > 4 || hand[i] > tile_limits[i]) {
          throw std::invalid_argument(std::format("Invalid number of tile_limits' at {}: {}", i, tile_limits[i]));
        }
      }

      if (m < 0 || m > 4) {
        throw std::invalid_argument(std::format("Invalid sum of hands's melds: {}", m));
      }

      if (mode == 0u || mode > 7u) {
        throw std::invalid_argument(std::format("Invalid caluculation mode: {}", mode));
      }
    }

    auto ret = T(MAX_SHT);

    if (mode & 1u) {
      chmin(ret, standard::calc_shanten<T>(hand, tile_limits, m));
    }

    if (m == 4) {
      if (mode & 2u) {
        if (four_tile_seven_pairs) {
          chmin(ret, seven_pairs::calc_shanten<T, true>(hand, tile_limits));
        }
        else {
          chmin(ret, seven_pairs::calc_shanten<T, false>(hand, tile_limits));
        }
      }

      if (mode & 4u) {
        chmin(ret, thirteen_orphans::calc_shanten<T>(hand, tile_limits));
      }
    }

    return ret == MAX_SHT ? std::nullopt : std::make_optional<T>(ret);
  }

  std::optional<int8_t> calc_shanten(const std::array<int, NUM_TIDS>& hand,
                                     const std::array<int, NUM_TIDS + 1>& tile_limits,
                                     const int m,
                                     unsigned int mode,
                                     const bool four_tile_seven_pairs,
                                     const bool check_hand)
  {
    return calc_shanten_impl<int8_t>(hand, tile_limits, m, mode, four_tile_seven_pairs, check_hand);
  }

  std::optional<Data> calc_shanten2(const std::array<int, NUM_TIDS>& hand,
                                    const std::array<int, NUM_TIDS + 1>& tile_limits,
                                    const int m,
                                    unsigned int mode,
                                    const bool four_tile_seven_pairs,
                                    const bool check_hand)
  {
    return calc_shanten_impl<Data>(hand, tile_limits, m, mode, four_tile_seven_pairs, check_hand);
  }

  std::array<std::vector<standard::Delta>, NUM_TIDS> standard::make_deltas()
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
