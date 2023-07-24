import std;
import test;

import cherry.board;
import cherry.move;
import cherry.squareIndex;

cherry::test::Test fenStartingPosition("FENStartingPosition", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	runner.expectEq(cherry::Piece::WhiteKing, board.at(cherry::SquareIndex("e1")));
	runner.expectEq(cherry::Piece::PieceNone, board.at(cherry::SquareIndex("h6")));
	runner.expectEq(cherry::SquareIndex("e1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("e8"), board.blackKing_);
	runner.expectEq(true, board.whiteToPlay_);
});

cherry::test::Test fenItalian("FENItalian", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("r1bq1rk1/pppp1ppp/2n2n2/2b1p3/2B1P3/2PP1N2/PP3PPP/RNBQ1RK1 b - - 2 6");
	runner.expectEq(cherry::Piece::BlackKnight, board.at(cherry::SquareIndex("c6")));
	runner.expectEq(false, board.blackKingsideCastle_);
	runner.expectEq(false, board.blackQueensideCastle_);
	runner.expectEq(false, board.whiteKingsideCastle_);
	runner.expectEq(false , board.whiteQueensideCastle_);
	runner.expectEq(cherry::SquareIndex("g1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("g8"), board.blackKing_);
	runner.expectEq(false, board.whiteToPlay_);
});

cherry::test::Test fenWinawer("FENWinawer", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("rnbq1rk1/pp3ppp/5n2/2Pp4/7P/P1PB4/2P2PP1/R1BQK1NR b KQ h3 0 9");
	runner.expectEq(false, board.blackKingsideCastle_);
	runner.expectEq(false, board.blackQueensideCastle_);
	runner.expectEq(true, board.whiteKingsideCastle_);
	runner.expectEq(true, board.whiteQueensideCastle_);
	runner.expectEq(cherry::SquareIndex("e1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("g8"), board.blackKing_);
	runner.expectEq(cherry::SquareIndex("h3"), board.enPassantTarget_);
});

cherry::test::Test movesItalian("MovesItalian", [](cherry::test::TestRunner& runner) {
	cherry::Board board = cherry::startingPosition;
	board.makeMove(cherry::Move("e2e4"));
	board.makeMove(cherry::Move("e7e5"));
	board.makeMove(cherry::Move("g1f3"));
	board.makeMove(cherry::Move("b8c6"));
	board.makeMove(cherry::Move("f1c4"));
	board.makeMove(cherry::Move("g8f6"));
	board.makeMove(cherry::Move("d2d3"));
	board.makeMove(cherry::Move("f8c5"));
	board.makeMove(cherry::Move("c2c3"));
	board.makeMove(cherry::Move("e8g8"));
	runner.expectEq(cherry::Board("r1bq1rk1/pppp1ppp/2n2n2/2b1p3/2B1P3/2PP1N2/PP3PPP/RNBQK2R w KQ - 1 6"), board);
	runner.expectEq(cherry::SquareIndex("e1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("g8"), board.blackKing_);
	});

cherry::test::Test movesPetroff("MovePetroff", [](cherry::test::TestRunner& runner) {
	cherry::Board board = cherry::startingPosition;
	board.makeMove(cherry::Move("e2e4"));
	board.makeMove(cherry::Move("e7e5"));
	board.makeMove(cherry::Move("g1f3"));
	board.makeMove(cherry::Move("g8f6"));
	board.makeMove(cherry::Move("d2d4"));
	board.makeMove(cherry::Move("e5d4"));
	board.makeMove(cherry::Move("e4e5"));
	board.makeMove(cherry::Move("f6e4"));
	board.makeMove(cherry::Move("d1d4"));
	board.makeMove(cherry::Move("d7d5"));
	runner.expectEq(cherry::Board("rnbqkb1r/ppp2ppp/8/3pP3/3Qn3/5N2/PPP2PPP/RNB1KB1R w KQkq d6 0 6"), board);
	board.makeMove(cherry::Move("e5d6"));
	runner.expectEq(cherry::Board("rnbqkb1r/ppp2ppp/3P4/8/3Qn3/5N2/PPP2PPP/RNB1KB1R b KQkq - 0 6"), board);
	runner.expectEq(cherry::SquareIndex("e1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("e8"), board.blackKing_);
	});

cherry::test::Test movesPhilidor("MovePhilidor", [](cherry::test::TestRunner& runner) {
	cherry::Board board = cherry::startingPosition;
	board.makeMove(cherry::Move("e2e4"));
	board.makeMove(cherry::Move("e7e5"));
	board.makeMove(cherry::Move("g1f3"));
	board.makeMove(cherry::Move("d7d6"));
	board.makeMove(cherry::Move("d2d4"));
	board.makeMove(cherry::Move("e5d4"));
	board.makeMove(cherry::Move("f3d4"));
	board.makeMove(cherry::Move("g8f6"));
	board.makeMove(cherry::Move("b1c3"));
	board.makeMove(cherry::Move("f8e7"));
	board.makeMove(cherry::Move("c1f4"));
	board.makeMove(cherry::Move("e8g8"));
	board.makeMove(cherry::Move("d1d2"));
	board.makeMove(cherry::Move("d6d5"));
	board.makeMove(cherry::Move("e1c1"));
	board.makeMove(cherry::Move("d5e4"));
	board.makeMove(cherry::Move("f1c4"));
	board.makeMove(cherry::Move("c8g4"));
	board.makeMove(cherry::Move("f2f3"));
	board.makeMove(cherry::Move("e4f3"));
	board.makeMove(cherry::Move("h2h3"));
	board.makeMove(cherry::Move("c7c5"));
	board.makeMove(cherry::Move("h3g4"));
	board.makeMove(cherry::Move("d8d4"));
	board.makeMove(cherry::Move("c4d3"));
	board.makeMove(cherry::Move("f3g2"));
	board.makeMove(cherry::Move("c3d5"));
	board.makeMove(cherry::Move("g2h1q"));
	runner.expectEq(cherry::Board("rn3rk1/pp2bppp/5n2/2pN4/3q1BP1/3B4/PPPQ4/2KR3q w - - 0 15"), board);
	runner.expectEq(cherry::SquareIndex("c1"), board.whiteKing_);
	runner.expectEq(cherry::SquareIndex("g8"), board.blackKing_);
	});

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}
