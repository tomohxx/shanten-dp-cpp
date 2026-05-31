#include <benchmark/benchmark.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <mahjong/experimental/shanten_calculator.hpp>
#include <stdexcept>
#include <tuple>
#include <vector>
namespace me = mahjong::experimental;
using Record = std::tuple<std::array<int, me::NUM_TIDS>, int, int, int>;
using Dataset = std::vector<Record>;
constexpr int NUM_TILES = 14;
constexpr int NUM_RECORDS = 10000;
const auto data_dir = std::filesystem::path(__FILE__).parent_path() / "data";

const auto& make_tile_limits()
{
  static const auto tile_limits = me::make_tile_limits();
  return tile_limits;
}

void read(std::vector<Record>& dataset, const std::filesystem::path& path)
{
  std::ifstream fin(path);

  if (!fin) {
    throw std::runtime_error("failed to open benchmark data: " + path.string());
  }

  int tid;

  for (auto& [hand, standard, seven_pairs, thirteen_orphans] : dataset) {
    hand.fill(0);

    for (int i = 0; i < NUM_TILES; ++i) {
      fin >> tid;
      ++hand.at(tid);
    }

    fin >> standard >> thirteen_orphans >> seven_pairs;
  }
}

void verify(const std::vector<Record>& dataset)
{
  const auto& tile_limits = make_tile_limits();

  for (const auto& [hand, standard, seven_pairs, thirteen_orphans] : dataset) {
    if (me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 1u) != standard) {
      throw std::runtime_error("standard::calc_shanten validation failed");
    }

    if (me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 2u) != seven_pairs) {
      throw std::runtime_error("seven_pairs::calc_shanten validation failed");
    }

    if (me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 4u) != thirteen_orphans) {
      throw std::runtime_error("thirteen_orphans::calc_shanten validation failed");
    }
  }
}

std::vector<Dataset> load_datasets()
{
  std::vector<Dataset> datasets = {
      Dataset(NUM_RECORDS),
      Dataset(NUM_RECORDS),
      Dataset(NUM_RECORDS),
      Dataset(NUM_RECORDS),
  };

  read(datasets[0], data_dir / "p_normal_10000.txt");
  read(datasets[1], data_dir / "p_tin_10000.txt");
  read(datasets[2], data_dir / "p_hon_10000.txt");
  read(datasets[3], data_dir / "p_koku_10000.txt");

  for (const auto& dataset : datasets) {
    verify(dataset);
  }

  return datasets;
}

const Dataset& get_dataset(const std::size_t index)
{
  static const auto datasets = load_datasets();
  return datasets.at(index);
}

void BM_CalcShanten(benchmark::State& state)
{
  const auto& dataset = get_dataset(static_cast<std::size_t>(state.range(0)));
  const auto& tile_limits = make_tile_limits();

  for (auto _ : state) {
    for (const auto& record : dataset) {
      const auto& hand = std::get<0>(record);

      benchmark::DoNotOptimize(me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 1u));
      benchmark::DoNotOptimize(me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 2u));
      benchmark::DoNotOptimize(me::calc_shanten<int8_t>(hand, tile_limits, NUM_TILES / 3, 4u));
    }
  }
}

BENCHMARK(BM_CalcShanten)->Arg(0)->Name("normal");
BENCHMARK(BM_CalcShanten)->Arg(1)->Name("tinitu");
BENCHMARK(BM_CalcShanten)->Arg(2)->Name("honitu");
BENCHMARK(BM_CalcShanten)->Arg(3)->Name("kokusi");

BENCHMARK_MAIN();
