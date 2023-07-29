# CMakeList.txt : CMake project for CherryChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.

get_filename_component(TEST_FILE_PATH "input" ABSOLUTE)
configure_file("framework/test_path.h.in" "framework/test_path.h")

add_library(test_framework "framework/test_path.h")
target_sources(test_framework PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "framework/test.ixx")
target_include_directories(test_framework PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

function(cherry_test test_name test_path)
add_executable(${test_name} ${test_path})
target_link_libraries(${test_name} PRIVATE stdxx)
target_link_libraries(${test_name} PRIVATE test_framework)
target_link_libraries(${test_name} PRIVATE CherryChessSrc)
add_test(${test_name} ${test_name})
endfunction()

cherry_test(board_test "data/boardTest.ixx")
cherry_test(piece_test "data/pieceTest.ixx")
cherry_test(square_index_test "data/squareIndexTest.ixx")
cherry_test(tactics_test "data/tacticsTest.ixx")
cherry_test(move_enumeration_test "game/moveEnumerationTest.ixx")
