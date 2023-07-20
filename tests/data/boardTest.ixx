import std;
import test;

import cherry.board;
import cherry.squareIndex;

cherry::test::Test fenStartingPosition("FENStartingPosition", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	runner.expectEq(cherry::Piece::WhiteKing, board.at(cherry::SquareIndex("e1")));
	runner.expectEq(cherry::Piece::None, board.at(cherry::SquareIndex("h6")));
	runner.expectEq(true, board.whiteToPlay_);
});

cherry::test::Test fenItalian("FENItalian", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("r1bq1rk1/pppp1ppp/2n2n2/2b1p3/2B1P3/2PP1N2/PP3PPP/RNBQ1RK1 b - - 2 6");
	runner.expectEq(cherry::Piece::BlackKnight, board.at(cherry::SquareIndex("c6")));
	runner.expectEq(false, board.blackKingsideCastle_);
	runner.expectEq(false, board.blackQueensideCastle_);
	runner.expectEq(false, board.whiteKingsideCastle_);
	runner.expectEq(false , board.whiteQueensideCastle_);
	runner.expectEq(false, board.whiteToPlay_);
});

cherry::test::Test fenWinawer("FENWinawer", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("rnbq1rk1/pp3ppp/5n2/2Pp4/7P/P1PB4/2P2PP1/R1BQK1NR b KQ h3 0 9");
	runner.expectEq(false, board.blackKingsideCastle_);
	runner.expectEq(false, board.blackQueensideCastle_);
	runner.expectEq(true, board.whiteKingsideCastle_);
	runner.expectEq(true, board.whiteQueensideCastle_);
	runner.expectEq(cherry::SquareIndex("h3"), board.enPassantTarget_);
});

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}
