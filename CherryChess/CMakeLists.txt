# CMakeList.txt : CMake project for CherryChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_executable (CherryChess "CherryChess.ixx" )
target_link_libraries(CherryChess PRIVATE stdxx)
target_link_libraries(CherryChess PRIVATE CherryChessSrc)

set_property(TARGET CherryChess PROPERTY CXX_STANDARD 23)
