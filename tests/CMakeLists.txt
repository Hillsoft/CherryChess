# CMakeList.txt : CMake project for CherryChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.

function(cherry_test test_name test_path)
add_executable(${test_name} ${test_path})
target_link_libraries(${test_name} PRIVATE CherryChessSrc)
target_sources(${test_name} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "framework/test.ixx")
add_test(${test_name} ${test_name})
endfunction()

cherry_test(board_test "data/boardTest.ixx")
cherry_test(piece_test "data/pieceTest.ixx")
cherry_test(square_index_test "data/squareIndexTest.ixx")
cherry_test(move_enumeration_test "game/moveEnumerationTest.ixx")
