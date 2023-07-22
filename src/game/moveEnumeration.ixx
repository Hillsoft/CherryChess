module;

#include <assert.h>

export module cherry.moveEnumeration;

import cherry.board;
import cherry.move;
import cherry.piece;
import cherry.squareIndex;

namespace cherry {

	/* constexpr */ std::vector<Move> availableMovesRaw(Board const& board, PieceColor toPlay, std::vector<SquareIndex> const& opponentTargets) {
		assert(toPlay == PieceColor::White || toPlay == PieceColor::Black);

		constexpr std::array<std::pair<char, char>, 4> straight = { std::pair(1, 0), std::pair(-1, 0), std::pair(0, 1), std::pair(0, -1) };
		constexpr std::array<std::pair<char, char>, 4> diagonal = { std::pair(1, 1), std::pair(1, -1), std::pair(-1, 1), std::pair(-1, -1) };
		constexpr std::array<std::pair<char, char>, 8> knight = { std::pair(1, 2), std::pair(2, 1), std::pair(2, -1), std::pair(1, -2), std::pair(-1, -2), std::pair(-2, -1), std::pair(-2, 1), std::pair(-1, 2) };
		constexpr std::array<PieceType, 4> promotionTypes = { PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen };

		std::vector<Move> result;

		PieceColor opponent = toPlay == PieceColor::White ? PieceColor::Black : PieceColor::White;

		auto scan = [&](SquareIndex root, std::pair<char, char> step, size_t maxDist = 8) {
			auto& xStep = step.first;
			auto& yStep = step.second;
			SquareIndex current = root;
			char rawOffset = xStep + 8 * yStep;
			while (!(maxDist == 0
				|| (xStep + current.getFile() < 0)
				|| (xStep + current.getFile() > 7)
				|| (current.getRank() - yStep < 0)
				|| (current.getRank() - yStep > 7))) {
				current = SquareIndex(current.getRawIndex() + rawOffset);
				if (getPieceColor(board.at(current)) != toPlay) {
					result.push_back(Move(root, current));
				}
				if (board.at(current) != Piece::PieceNone) {
					return;
				}
				maxDist--;
			}
		};
		auto maybeWithPromotion = [&](SquareIndex root, SquareIndex target) {
			if (target.getRank() == 0 || target.getRank() == 7) {
				for (auto const& type : promotionTypes) {
					result.push_back(Move(root, target, type));
				}
			}
			else {
				result.push_back(Move(root, target));
			}
		};

		for (char i = 0; i < 64; i++) {
			if (getPieceColor(board.at(SquareIndex(i))) == toPlay) {
				switch (getPieceType(board.at(SquareIndex(i)))) {
				case PieceType::King:
					for (auto const& dir : straight) {
						scan(SquareIndex(i), dir, 1);
					}
					for (auto const& dir : diagonal) {
						scan(SquareIndex(i), dir, 1);
					}
					break;
				case PieceType::Rook:
					for (auto const& dir : straight) {
						scan(SquareIndex(i), dir);
					}
					break;
				case PieceType::Bishop:
					for (auto const& dir : diagonal) {
						scan(SquareIndex(i), dir);
					}
					break;
				case PieceType::Queen:
					for (auto const& dir : straight) {
						scan(SquareIndex(i), dir);
					}
					for (auto const& dir : diagonal) {
						scan(SquareIndex(i), dir);
					}
					break;
				case PieceType::Knight:
					for (auto const& dir : knight) {
						scan(SquareIndex(i), dir, 1);
					}
					break;
				case PieceType::Pawn:
					char step = toPlay == PieceColor::White ? -8 : 8;
					SquareIndex currentSquare = SquareIndex(i);
					SquareIndex nextSquare = SquareIndex(i + step);
					if (board.at(nextSquare) == Piece::PieceNone) {
						char nextRank = 7 - nextSquare.getRank();
						// Regular movement
						maybeWithPromotion(currentSquare, nextSquare);
						// Double movement, promotion is not possible here
						if (((nextRank == 2 && step > 0) || (nextRank == 5 && step < 0)) && board.at(SquareIndex(i + 2 * step)) == Piece::PieceNone) {
							result.push_back(Move(currentSquare, SquareIndex(i + 2 * step)));
						}
					}

					nextSquare = SquareIndex(i + step - 1);
					if (currentSquare.getFile() != 0 && (board.enPassantTarget_ == nextSquare || getPieceColor(board.at(nextSquare)) == opponent)) {
						maybeWithPromotion(currentSquare, nextSquare);
					}
					nextSquare = SquareIndex(i + step + 1);
					if (currentSquare.getFile() != 7 && (board.enPassantTarget_ == nextSquare || getPieceColor(board.at(nextSquare)) == opponent)) {
						maybeWithPromotion(currentSquare, nextSquare);
					}
					break;
				}
			}
		}

		// Castling
		if (toPlay == PieceColor::White) {
			if (board.whiteKingsideCastle_) {
				if (board.at(SquareIndex("f1")) == Piece::PieceNone && board.at(SquareIndex("g1")) == Piece::PieceNone) {
					bool castleBlocked = false;
					for (auto const& t : opponentTargets) {
						if (t == SquareIndex("e1") || t == SquareIndex("f1") || t == SquareIndex("g1")) {
							castleBlocked = true;
							break;
						}
					}
					if (!castleBlocked) {
						result.push_back(Move("e1g1"));
					}
				}
			}
			if (board.whiteQueensideCastle_) {
				if (board.at(SquareIndex("b1")) == Piece::PieceNone && board.at(SquareIndex("c1")) == Piece::PieceNone && board.at(SquareIndex("d1")) == Piece::PieceNone) {
					bool castleBlocked = false;
					for (auto const& t : opponentTargets) {
						if (t == SquareIndex("e1") || t == SquareIndex("d1") || t == SquareIndex("c1")) {
							castleBlocked = true;
							break;
						}
					}
					if (!castleBlocked) {
						result.push_back(Move("e1c1"));
					}
				}
			}
		}
		if (toPlay == PieceColor::Black) {
			if (board.blackKingsideCastle_) {
				if (board.at(SquareIndex("f8")) == Piece::PieceNone && board.at(SquareIndex("g8")) == Piece::PieceNone) {
					bool castleBlocked = false;
					for (auto const& t : opponentTargets) {
						if (t == SquareIndex("e8") || t == SquareIndex("f8") || t == SquareIndex("g8")) {
							castleBlocked = true;
							break;
						}
					}
					if (!castleBlocked) {
						result.push_back(Move("e8g8"));
					}
				}
			}
			if (board.blackQueensideCastle_) {
				if (board.at(SquareIndex("b8")) == Piece::PieceNone && board.at(SquareIndex("c8")) == Piece::PieceNone && board.at(SquareIndex("d8")) == Piece::PieceNone) {
					bool castleBlocked = false;
					for (auto const& t : opponentTargets) {
						if (t == SquareIndex("e8") || t == SquareIndex("d8") || t == SquareIndex("c8")) {
							castleBlocked = true;
							break;
						}
					}
					if (!castleBlocked) {
						result.push_back(Move("e8c8"));
					}
				}
			}
		}

		return result;
	}

	export /* constexpr */ std::vector<Move> availableMoves(Board const& board) {
		std::vector<Move> opponentMoves = availableMovesRaw(board, board.whiteToPlay_ ? PieceColor::Black : PieceColor::White, {});
		std::vector<SquareIndex> opponentTargets;
		for (auto const& move : opponentMoves) {
			opponentTargets.push_back(move.to_);
		}

		return availableMovesRaw(board, board.whiteToPlay_ ? PieceColor::White : PieceColor::Black, opponentTargets);
	}

} // namespace
