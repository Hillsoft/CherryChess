cmake_minimum_required (VERSION 3.12)

add_library(CherryChessBenchmark)
target_link_libraries(CherryChessBenchmark PRIVATE stdxx)
target_link_libraries(CherryChessBenchmark PRIVATE CherryChessSrc)

target_sources(CherryChessBenchmark PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "common/benchmark.ixx")

function(cherry_bench bench_name bench_path)
add_executable(${bench_name} ${bench_path})
target_link_libraries(${bench_name} PRIVATE stdxx)
target_link_libraries(${bench_name} PRIVATE CherryChessSrc)
target_link_libraries(${bench_name} PRIVATE CherryChessBenchmark)
endfunction()

cherry_bench("move_enumeration_benchmark" "benchmarks/moveEnumerationBenchmark.ixx")
