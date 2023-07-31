import std;
import test;

import cherry.board;
import cherry.moveEnumeration;
import cherry.squareIndex;

void compareResults(cherry::test::TestRunner& runner, cherry::MoveEnumerationResult& expected, cherry::MoveEnumerationResult& actual) {
	std::sort(expected.others.begin(), expected.others.end());
	std::sort(actual.others.begin(), actual.others.end());

	runner.expectEq(expected.others, actual.others);
}

cherry::test::Test moveKings("MoveKings", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/8/2K5/8/8/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveKnight("MoveKnight", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/8/2K2n2/3P4/8/8 b - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("f4d3"),
			// King moves
			cherry::Move("g6f7"),
			cherry::Move("g6g7"),
			cherry::Move("g6h7"),
			cherry::Move("g6f6"),
			cherry::Move("g6h6"),
			cherry::Move("g6f5"),
			cherry::Move("g6g5"),
			cherry::Move("g6h5"),
			// Knight moves
			cherry::Move("f4d5"),
			cherry::Move("f4e6"),
			cherry::Move("f4h5"),
			cherry::Move("f4h3"),
			cherry::Move("f4g2"),
			cherry::Move("f4e2")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveRook("MoveRook", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/4p3/6k1/8/2K1R3/8/8/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("e4e6"),
			cherry::Move("e4g4"),
			cherry::Move("e4e7"),
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Rook moves
			cherry::Move("e4d4"),
			cherry::Move("e4e5"),
			cherry::Move("e4f4"),
			cherry::Move("e4h4"),
			cherry::Move("e4e3"),
			cherry::Move("e4e2"),
			cherry::Move("e4e1")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveBishop("MoveBishop", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/3P4/6k1/5b2/2K5/8/8/8 b - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("f5e6"),
			cherry::Move("f5d3"),
			cherry::Move("f5d7"),
			// King moves
			cherry::Move("g6f7"),
			cherry::Move("g6g7"),
			cherry::Move("g6h7"),
			cherry::Move("g6f6"),
			cherry::Move("g6h6"),
			cherry::Move("g6g5"),
			cherry::Move("g6h5"),
			// Bishop moves
			cherry::Move("f5e4"),
			cherry::Move("f5c2"),
			cherry::Move("f5b1"),
			cherry::Move("f5g4"),
			cherry::Move("f5h3")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveQueen("MoveQueen", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/8/2K3p1/8/2p1Q3/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("e2c2"),
			cherry::Move("e2g4"),
			cherry::Move("e2e4"),
			cherry::Move("e2e6"),
			cherry::Move("e2e8"),
			cherry::Move("e2d3"),
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Queen moves
			cherry::Move("e2d2"),
			cherry::Move("e2e3"),
			cherry::Move("e2e5"),
			cherry::Move("e2e7"),
			cherry::Move("e2f2"),
			cherry::Move("e2g2"),
			cherry::Move("e2h2"),
			cherry::Move("e2e1"),
			cherry::Move("e2d1"),
			cherry::Move("e2f3"),
			cherry::Move("e2f1")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveWhitePawn("MoveWhitePawn", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/5p2/2KP1P2/8/8/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Pawn moves
			cherry::Move("d4d5"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveBlackPawn("MoveBlackPawn", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/5p1p/2K2P2/8/8/8 b - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			// King moves
			cherry::Move("g6f7"),
			cherry::Move("g6g7"),
			cherry::Move("g6h7"),
			cherry::Move("g6f6"),
			cherry::Move("g6h6"),
			// Pawn moves
			cherry::Move("h5h4"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveWhitePawnStart("MoveWhitePawnStart", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/8/2K3p1/p7/P1P1P1P1/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Pawn moves
			cherry::Move("c2c3"),
			cherry::Move("e2e3"),
			cherry::Move("e2e4"),
			cherry::Move("g2g3"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveWhitePawnCaptures("MoveWhitePawnCaptures", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/pp4pp/PP4PP/K7/8/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("g4h5"),
			// Pawn moves
			cherry::Move("a4b5"),
			cherry::Move("b4a5"),
			cherry::Move("h4g5"),
			// King moves
			cherry::Move("a3b3"),
			cherry::Move("a3a2"),
			cherry::Move("a3b2"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveWhitePromotion("MoveWhitePromotion", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("5n2/4P3/6k1/8/2K5/8/8/8 w - - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("e7f8n"),
			cherry::Move("e7e8q"),
			cherry::Move("e7e8b"),
			cherry::Move("e7f8q"),
			cherry::Move("e7f8r"),
			cherry::Move("e7f8b"),
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Pawn moves
			cherry::Move("e7e8r"),
			cherry::Move("e7e8n"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test moveWhiteEnPassant("MoveWhiteEnPassant", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/4Pp2/2K5/8/8/8 w - f6 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("e5f6"),
			// King moves
			cherry::Move("c4b5"),
			cherry::Move("c4c5"),
			cherry::Move("c4d5"),
			cherry::Move("c4b4"),
			cherry::Move("c4d4"),
			cherry::Move("c4b3"),
			cherry::Move("c4c3"),
			cherry::Move("c4d3"),
			// Pawn moves
			cherry::Move("e5e6"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test movesStartingPosition("MoveStarting", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult  expectedMoves = {
		.others = {
			cherry::Move("a2a3"),
			cherry::Move("a2a4"),
			cherry::Move("b2b3"),
			cherry::Move("b2b4"),
			cherry::Move("c2c3"),
			cherry::Move("c2c4"),
			cherry::Move("d2d3"),
			cherry::Move("d2d4"),
			cherry::Move("e2e3"),
			cherry::Move("e2e4"),
			cherry::Move("f2f3"),
			cherry::Move("f2f4"),
			cherry::Move("g2g3"),
			cherry::Move("g2g4"),
			cherry::Move("h2h3"),
			cherry::Move("h2h4"),
			cherry::Move("b1a3"),
			cherry::Move("b1c3"),
			cherry::Move("g1f3"),
			cherry::Move("g1h3"),
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

cherry::test::Test movesCastling("MoveCastling", [](cherry::test::TestRunner& runner) {
	cherry::Board const board("8/8/6k1/8/8/7p/7P/4K2R w K - 0 1");
	auto moves = cherry::availableMoves(board);
	cherry::MoveEnumerationResult expectedMoves = {
		.others = {
			cherry::Move("h1g1"),
			// King moves
			cherry::Move("e1d1"),
			cherry::Move("e1d2"),
			cherry::Move("e1e2"),
			cherry::Move("e1f2"),
			cherry::Move("e1f1"),
			// Castling
			cherry::Move("e1g1"),
			// Rook moves
			cherry::Move("h1f1")
		}
	};

	compareResults(runner, expectedMoves, moves);
	});

// We intentionally don't test moving into check; this is always a bad move

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}
