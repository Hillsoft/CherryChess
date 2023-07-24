export module cherry.positionEval;

import std;
import cherry.board;
import cherry.evaluation;
import cherry.moveEnumeration;

export namespace cherry {

	constexpr std::array<int, 3> colorValues = { 0, 1, -1 };
	constexpr std::array<int, 7> pieceValues = { 0, 100, 500, 300, 310, 900, 10000 };

	 Evaluation evaluatePosition(Board const& position) {
		int runningTotal = 0;
		for (auto const& piece : position.data_) {
			runningTotal += colorValues[(size_t)getPieceColor(piece)] * pieceValues[(size_t)getPieceType(piece)];
		}
		runningTotal += 2 * colorValues[(size_t)getPieceColor(position.at(SquareIndex("d4")))];
		runningTotal += 2 * colorValues[(size_t)getPieceColor(position.at(SquareIndex("e4")))];
		runningTotal += 2 * colorValues[(size_t)getPieceColor(position.at(SquareIndex("d5")))];
		runningTotal += 2 * colorValues[(size_t)getPieceColor(position.at(SquareIndex("e5")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("c3")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("c4")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("c5")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("c6")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("f3")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("f4")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("f5")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("f6")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("d6")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("e6")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("d3")))];
		runningTotal += colorValues[(size_t)getPieceColor(position.at(SquareIndex("e3")))];
		return Evaluation(Evaluation::CPTag(), runningTotal * (position.whiteToPlay_ ? 1 : -1));
	 }

	 Evaluation terminalEval(Board position) {
		 position.whiteToPlay_ = !position.whiteToPlay_;
		 if (isIllegalDueToCheck(position)) {
			 return Evaluation(Evaluation::MateTag(), false, 0);
		 }
		 return Evaluation(Evaluation::CPTag(), 0);
	 }

} // namespace cherry
