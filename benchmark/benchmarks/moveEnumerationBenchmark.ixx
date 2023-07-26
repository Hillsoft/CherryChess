import std;
import benchmark;

import cherry.board;
import cherry.move;
import cherry.moveEnumeration;

cherry::Board currentBoard;
cherry::MoveEnumerationResult result;

void run(std::string_view name, cherry::Board board) {
	cherry::benchmark::runBenchmark(name, [&]() {
		result = cherry::availableMoves(board);
		});
}

int main() {
	currentBoard = cherry::startingPosition;
	run("StartingPosition", currentBoard);
	std::cin.ignore();
	return 0;
}
