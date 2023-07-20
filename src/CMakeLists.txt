# CMakeList.txt : CMake project for CherryChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_library (CherryChessSrc)
target_link_libraries(CherryChessSrc PRIVATE stdxx)

target_sources(CherryChessSrc PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "Data/board.ixx"  "Data/squareIndex.ixx" "Data/piece.ixx")
