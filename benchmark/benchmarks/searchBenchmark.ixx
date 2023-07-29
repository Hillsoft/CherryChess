import std;
import benchmark;

import cherry.board;
import cherry.evaluation;
import cherry.inlineStack;
import cherry.move;
import cherry.searchWorker;

cherry::Board currentBoard;
std::tuple<cherry::Evaluation, cherry::Move> result = std::tuple(cherry::Evaluation(cherry::Evaluation::CPTag(), 0), cherry::Move());

void run(std::string_view name, cherry::Board board) {
	cherry::benchmark::runBenchmark(name, [&]() {
		cherry::SearchWorker searcher;
		cherry::InlineStack<cherry::Board, 512> history;
		result = searcher.recursiveSearch(board, history, cherry::worstEval, cherry::bestEval, 4, 8);
		});
}

int main() {
	currentBoard = cherry::startingPosition;
	run("StartingPosition", currentBoard);
	std::cin.ignore();
	return 0;
}
