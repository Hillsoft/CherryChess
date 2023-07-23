export module cherry.positionEval;

import std;
import cherry.board;

export namespace cherry {

	constexpr std::array<int, 3> colorValues = { 0, 1, -1 };
	constexpr std::array<int, 7> pieceValues = { 0, 100, 500, 300, 310, 900, 10000 };

	int evaluatePosition(Board const& position) {
		int runningTotal = 0;
		for (auto const& piece : position.data_) {
			runningTotal += colorValues[(size_t)getPieceColor(piece)] * pieceValues[(size_t)getPieceType(piece)];
		}
		return runningTotal;
	}

} // namespace cherry
