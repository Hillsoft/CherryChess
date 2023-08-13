export module cherry.positionEval;

import std;
import cherry.board;
import cherry.evaluation;
import cherry.evalConstants;
import cherry.moveEnumeration;
import cherry.piece;

export namespace cherry {

	Evaluation evaluatePosition(Board const& position) {
		int totalMaterial = 0;
		int runningTotal = 0;
		for (auto const& piece : position.data_) {
			runningTotal += colorValues[(size_t)getPieceColor(piece)] * pieceValues[(size_t)getPieceType(piece)];
			totalMaterial += pieceValues[(size_t)getPieceType(piece)];
		}
		for (char index = 0; index < 64; index++) {
			char adjusted = position.whiteToPlay_ ? index : SquareIndex(index).flipVertical().getRawIndex();
			Piece piece = position.at(SquareIndex(index));
			runningTotal += colorValues[(size_t)getPieceColor(piece)] * pieceAffinity[64 * (size_t)getPieceType(piece) + (size_t)adjusted];
		}

		// Give some positive eval to having central kings during the endgame
		if (totalMaterial < 2000) {
			runningTotal += std::min<char>(position.whiteKing_.getRank(), (char)7 - position.whiteKing_.getRank());
			runningTotal += std::min<char>(position.whiteKing_.getFile(), (char)7 - position.whiteKing_.getFile());
			runningTotal -= std::min<char>(position.blackKing_.getRank(), (char)7 - position.blackKing_.getRank());
			runningTotal -= std::min<char>(position.blackKing_.getFile(), (char)7 - position.blackKing_.getFile());
		}

		if (isInCheck(position)) {
			if (position.whiteToPlay_) {
				runningTotal -= 200;
			}
			else {
				runningTotal += 200;
			}
		}

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
