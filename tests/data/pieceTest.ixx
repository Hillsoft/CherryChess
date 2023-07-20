import std;
import test;

import cherry.piece;

cherry::test::Test pieceColor("PieceColor", [](cherry::test::TestRunner& runner) {
	runner.expectEq(cherry::PieceColor::White, cherry::getPieceColor(cherry::Piece::WhiteBishop));
	runner.expectEq(cherry::PieceColor::Black, cherry::getPieceColor(cherry::Piece::BlackKing));
});

cherry::test::Test pieceType("PieceType", [](cherry::test::TestRunner& runner) {
	runner.expectEq(cherry::PieceType::King, cherry::getPieceType(cherry::Piece::WhiteKing));
	runner.expectEq(cherry::PieceType::Rook, cherry::getPieceType(cherry::Piece::BlackRook));
});

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}
