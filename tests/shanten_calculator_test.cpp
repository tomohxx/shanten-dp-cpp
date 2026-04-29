#include <gtest/gtest.h>
#include <mahjong/experimental/shanten_calculator.hpp>
using namespace mahjong::experimental;

class CalcShantenTest : public ::testing::Test {
protected:
  CalcShantenTest() : deltas(make_deltas()) {}

  const decltype(make_deltas()) deltas;
};

TEST_F(CalcShantenTest, ClosedHand)
{
  // 123m245779p13555z
  std::array<int, NUM_TIDS> hand = {
      1, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 1, 0, 1, 1, 0, 2, 0, 1, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 3, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, InsufficientBlocks4433)
{
  // 11112222333444z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 3, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 1);
}

TEST_F(CalcShantenTest, InsufficientBlocks4442i)
{
  // 11m111122223333z
  std::array<int, NUM_TIDS> hand = {
      2, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, InsufficientBlocks4442ii)
{
  // 23m111122223333z
  std::array<int, NUM_TIDS> hand = {
      0, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, InsufficientBlocks4333)
{
  // 1111222333444z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 3, 3, 3, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 1);
}

TEST_F(CalcShantenTest, InsufficientBlocks4432i)
{
  // 11m11112222333z
  std::array<int, NUM_TIDS> hand = {
      2, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, InsufficientBlocks4432ii)
{
  // 23m11112222333z
  std::array<int, NUM_TIDS> hand = {
      0, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 3, 0, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, InsufficientBlocks4441)
{
  // 1111222233334z
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      4, 4, 4, 1, 0, 0, 0        // jihai
  };

  const auto shanten = calc_shanten(hand, make_tile_limits(), deltas, 4, false);

  EXPECT_EQ(shanten, 3);
}

TEST_F(CalcShantenTest, OpenHand1)
{
  // 13m123456p11s[[2222m]]
  std::array<int, NUM_TIDS> hand = {
      1, 0, 1, 0, 0, 0, 0, 0, 0, // manzu
      1, 1, 1, 1, 1, 1, 0, 0, 0, // pinzu
      2, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[1] = 0; // 2m

  const auto shanten = calc_shanten(hand, tile_limits, deltas, 3, false);

  EXPECT_EQ(shanten, 1);
}

TEST_F(CalcShantenTest, OpenHand2)
{
  // 9m9p569s[999m][999p][999s]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 1, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 1, // pinzu
      0, 0, 0, 0, 1, 1, 0, 0, 1, // souzu
      0, 0, 0, 0, 0, 0, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[8] = 1;  // 9m
  tile_limits[17] = 1; // 9p
  tile_limits[26] = 1; // 9s

  const auto shanten = calc_shanten(hand, tile_limits, deltas, 1, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, OpenHand3)
{
  // 11567z[777z][666z][555z]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      2, 0, 0, 0, 1, 1, 1        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[33] = 1; // 7z
  tile_limits[32] = 1; // 6z
  tile_limits[31] = 1; // 5z

  const auto shanten = calc_shanten(hand, tile_limits, deltas, 1, false);

  EXPECT_EQ(shanten, 2);
}

TEST_F(CalcShantenTest, OpenHand4)
{
  // 13556z[111z][333z][666z]
  std::array<int, NUM_TIDS> hand = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, // manzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // pinzu
      0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
      1, 0, 1, 0, 2, 1, 0        // jihai
  };

  auto tile_limits = make_tile_limits();

  tile_limits[27] = 1; // 1z
  tile_limits[29] = 1; // 3z
  tile_limits[32] = 1; // 6z

  const auto shanten = calc_shanten(hand, tile_limits, deltas, 1, false);

  EXPECT_EQ(shanten, 2);
}
