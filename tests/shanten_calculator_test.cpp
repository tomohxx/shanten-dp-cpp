#include <gtest/gtest.h>
#include <mahjong/experimental/shanten_calculator.hpp>
using namespace mahjong::experimental;

namespace {
  constexpr int m1 = 0;
  constexpr int m2 = 1;
  constexpr int m3 = 2;
  constexpr int m4 = 3;
  constexpr int m5 = 4;
  constexpr int m6 = 5;
  constexpr int m7 = 6;
  constexpr int m8 = 7;
  constexpr int m9 = 8;
  constexpr int p1 = 9;
  constexpr int p2 = 10;
  constexpr int p3 = 11;
  constexpr int p4 = 12;
  constexpr int p5 = 13;
  constexpr int p6 = 14;
  constexpr int p7 = 15;
  constexpr int p8 = 16;
  constexpr int p9 = 17;
  constexpr int s1 = 18;
  constexpr int s2 = 19;
  constexpr int s3 = 20;
  constexpr int s4 = 21;
  constexpr int s5 = 22;
  constexpr int s6 = 23;
  constexpr int s7 = 24;
  constexpr int s8 = 25;
  constexpr int s9 = 26;
  constexpr int z1 = 27;
  constexpr int z2 = 28;
  constexpr int z3 = 29;
  constexpr int z4 = 30;
  constexpr int z5 = 31;
  constexpr int z6 = 32;
  constexpr int z7 = 33;
}

TEST(CalcShantenTest, InvalidHandError)
{
  std::array<int, NUM_TIDS> hand = {
      5, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  EXPECT_THROW(calc_shanten(hand, make_tile_limits(), 4, 7u, false, true), std::invalid_argument);
}

TEST(CalcShantenError, InvalidTileLimitsError)
{
  std::array<int, NUM_TIDS> hand = {
      1, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  EXPECT_THROW(calc_shanten(hand, std::array<int, NUM_TIDS + 1>{}, 4, 7u, false, true), std::invalid_argument);
}

TEST(CalcShantenTest, InvalidMeldsError)
{
  std::array<int, NUM_TIDS> hand = {};

  EXPECT_THROW(calc_shanten(hand, make_tile_limits(), 5, 7u, false, true), std::invalid_argument);
}

TEST(CalcShantenTest, InvalidModeError)
{
  std::array<int, NUM_TIDS> hand = {};

  EXPECT_THROW(calc_shanten(hand, make_tile_limits(), 4, 8u, false, true), std::invalid_argument);
}

TEST(CalcShantenTest, ClosedHand)
{
  // 123m245779p13555z
  std::array<int, NUM_TIDS> hand = {
      1, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 1, 0, 1, 1, 0, 2, 0, 1, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 3, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, CannotWin)
{
  // 123m245779p13555z
  std::array<int, NUM_TIDS> hand = {
      1, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 1, 0, 1, 1, 0, 2, 0, 1, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 3, 0, 0        // jihai
  };

  decltype(make_tile_limits()) tile_limits{};

  std::copy(hand.cbegin(), hand.cend(), tile_limits.begin());

  const auto shanten = calc_shanten(hand, tile_limits, 4, 7u, false, true);

  EXPECT_FALSE(shanten.has_value());
}

TEST(CalcShantenTest, InsufficientBlocks4433)
{
  // 11112222333444z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 3, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 1);
}

TEST(CalcShantenTest, InsufficientBlocks4442i)
{
  // 11m111122223333z
  std::array<int, NUM_TIDS> hand = {
      2, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, InsufficientBlocks4442ii)
{
  // 23m111122223333z
  std::array<int, NUM_TIDS> hand = {
      0, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, InsufficientBlocks4333)
{
  // 1111222333444z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 3, 3, 3, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 1);
}

TEST(CalcShantenTest, InsufficientBlocks4432i)
{
  // 11m11112222333z
  std::array<int, NUM_TIDS> hand = {
      2, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, InsufficientBlocks4432ii)
{
  // 23m11112222333z
  std::array<int, NUM_TIDS> hand = {
      0, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, InsufficientBlocks4441)
{
  // 1111222233334z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 1, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(shanten, 3);
}

TEST(CalcShantenTest, OpenHand1)
{
  // 13m123456p11s[[2222m]]
  std::array<int, NUM_TIDS> hand = {
      1, 0, 1, 0, 0, 0, 0, 0, 0, // manzu
      1, 1, 1, 1, 1, 1, 0, 0, 0, // pinzu
      2, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[m2] = 0; // 2m

  const auto shanten = calc_shanten(hand, tile_limits, 3, 7u, false, true);

  EXPECT_EQ(shanten, 1);
}

TEST(CalcShantenTest, OpenHand2)
{
  // 9m9p569s[999m][999p][999s]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 1, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 1, // pinzu
      0, 0, 0, 0, 1, 1, 0, 0, 1, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[m9] = 1;
  tile_limits[p9] = 1;
  tile_limits[s9] = 1;

  const auto shanten = calc_shanten(hand, tile_limits, 1, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, OpenHand3)
{
  // 11567z[777z][666z][555z]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      2, 0, 0, 0, 1, 1, 1        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[z7] = 1;
  tile_limits[z6] = 1;
  tile_limits[z5] = 1;

  const auto shanten = calc_shanten(hand, tile_limits, 1, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, OpenHand4)
{
  // 13556z[111z][333z][666z]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 2, 1, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[z1] = 1;
  tile_limits[z3] = 1;
  tile_limits[z6] = 1;

  const auto shanten = calc_shanten(hand, tile_limits, 1, 7u, false, true);

  EXPECT_EQ(shanten, 2);
}

TEST(CalcShantenTest, FourTileSevenPairs)
{
  // 11223344556666z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      2, 2, 2, 2, 2, 4, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), 1, 7u, true, true);

  EXPECT_EQ(shanten, -1);
}

TEST(CalcShantenTest, DiscardsAndWaits)
{
  // 123m245779p13555z
  std::array<int, NUM_TIDS> hand = {
      1, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 1, 0, 1, 1, 0, 2, 0, 1, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 3, 0, 0        // jihai
  };

  const auto data = calc_shanten2(hand, make_tile_limits(), 4, 7u, false, true);

  EXPECT_EQ(data->shanten, 2);
  EXPECT_EQ(data->discards, 0b0010101'000000000'101011010'000000000);
  EXPECT_EQ(data->waits, 0b0000101'000000000'111111111'000000000);
}
