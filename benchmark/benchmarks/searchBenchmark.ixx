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
		searcher.iterativeDeepening(board, history, 4);
		});
}



int main() {
	currentBoard = cherry::startingPosition;
	run("StartingPosition", currentBoard);
	run("Midgame", cherry::Board("r1bqk2r/1p3ppp/p3p3/3pn3/2PNn3/B3Q3/P1P1BPPP/R4RK1 b kq - 2 12"));
	run("Endgame", cherry::Board("8/1p6/8/6p1/1kP1p3/1P1pP2P/6P1/5K2 b - - 0 36"));
	std::cin.ignore();
	return 0;
}
