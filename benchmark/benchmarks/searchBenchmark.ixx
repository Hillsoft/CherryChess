import std;
import benchmark;

import cherry.board;
import cherry.evaluation;
import cherry.move;
import cherry.searchManager;

cherry::Board currentBoard;
std::tuple<cherry::Evaluation, cherry::Move, int> result = std::tuple(cherry::Evaluation(cherry::Evaluation::CPTag(), 0), cherry::Move(), 0);

void run(std::string_view name, cherry::Board board) {
	cherry::benchmark::runBenchmark(name, [&]() {
		result = cherry::recursiveSearch(board, cherry::worstEval, cherry::bestEval, 4);
		});
}

int main() {
	currentBoard = cherry::startingPosition;
	run("StartingPosition", currentBoard);
	std::cin.ignore();
	return 0;
}
