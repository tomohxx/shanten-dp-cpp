cmake_minimum_required(VERSION 3.12)

project(shanten-dp-cpp LANGUAGES CXX)

include(GNUInstallDirs)

find_package(GTest QUIET)
find_package(benchmark QUIET)

option(FIX_RANDOM_SEED "FIX_RANDOM_SEED" OFF)

add_library(compile_settings INTERFACE)
target_compile_features(compile_settings INTERFACE cxx_std_20)
target_compile_options(compile_settings INTERFACE
  $<$<CONFIG:Debug>:-O0 -Wall -Wextra -g>
  $<$<CONFIG:Release>:-O3 -Wall -Wextra>)
target_include_directories(compile_settings INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(shanten_calculator src/shanten_calculator.cpp)
target_link_libraries(shanten_calculator compile_settings)

install(TARGETS shanten_calculator compile_settings EXPORT MahjongExperimental
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/experimental
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/experimental)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/mahjong DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT MahjongExperimental
  FILE MahjongExperimentalConfig.cmake
  NAMESPACE MahjongExperimental::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mahjong)

if(GTest_FOUND)
  enable_testing()

  add_executable(shanten_calculator_test tests/shanten_calculator_test.cpp)
  target_link_libraries(shanten_calculator_test PRIVATE shanten_calculator GTest::GTest GTest::Main)

  include(GoogleTest)
  gtest_discover_tests(shanten_calculator_test)
else()
  message(STATUS "GTest not found; tests are disabled")
endif()

if(benchmark_FOUND)
  add_executable(shanten_calculator_bench benches/shanten_calculator_bench.cpp)
  target_link_libraries(shanten_calculator_bench PRIVATE shanten_calculator benchmark::benchmark)
else()
  message(STATUS "benchmark not found; benchmarks are disabled")
endif()
