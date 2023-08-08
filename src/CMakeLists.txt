# CMakeList.txt : CMake project for CherryChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_library (CherryChessSrc)
target_link_libraries(CherryChessSrc PRIVATE stdxx)

target_sources(CherryChessSrc PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "Data/board.ixx"  "Data/squareIndex.ixx" "Data/piece.ixx" "Data/move.ixx" "game/moveEnumeration.ixx" "uci/commands/toEngine/cmduci.ixx" "uci/commands/toEngine.ixx" "uci/interfaceManager.ixx" "utils/concurrentQueue.ixx" "uci/commands/toEngine/cmdquit.ixx" "uci/commands/fromEngine/cmduciok.ixx" "uci/commands/fromEngine/cmdid.ixx" "uci/commandEmitter.ixx" "uci/commands/toEngine/cmdisready.ixx" "uci/commands/fromEngine/cmdreadyok.ixx" "game/searchManager.ixx" "uci/commands/toEngine/cmdposition.ixx" "uci/commands/toEngine/cmdgo.ixx" "uci/commands/fromEngine/cmdbestmove.ixx" "game/positionEval.ixx" "uci/commands/fromEngine/cmdinfo.ixx" "Data/evaluation.ixx" "game/searchWorker.ixx" "uci/commands/toEngine/cmdstop.ixx" "utils/inlineStack.ixx" "Data/evaluationRange.ixx" "Data/searchResult.ixx" "Data/transpositionTable.ixx" "utils/bitwiseHash.ixx")
