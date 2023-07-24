module;

#include <assert.h>

export module cherry.moveEnumeration;

import cherry.board;
import cherry.move;
import cherry.piece;
import cherry.squareIndex;

namespace cherry {

	namespace {
		constexpr std::array<std::pair<char, char>, 4> straight = { std::pair(1, 0), std::pair(-1, 0), std::pair(0, 1), std::pair(0, -1) };
		constexpr std::array<std::pair<char, char>, 4> diagonal = { std::pair(1, 1), std::pair(1, -1), std::pair(-1, 1), std::pair(-1, -1) };
		constexpr std::array<std::pair<char, char>, 8> knight = { std::pair(1, 2), std::pair(2, 1), std::pair(2, -1), std::pair(1, -2), std::pair(-1, -2), std::pair(-2, -1), std::pair(-2, 1), std::pair(-1, 2) };
		constexpr std::array<PieceType, 4> promotionTypes = { PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen };
	} // namespace

	export bool isIllegalDueToCheck(Board const& board);

	/* constexpr */ std::vector<Move> availableMovesRaw(Board const& board, PieceColor toPlay, std::vector<SquareIndex> const& opponentTargets, bool enforceNoCheck = true) {
		assert(toPlay == PieceColor::White || toPlay == PieceColor::Black);

		std::vector<Move> result;

		PieceColor opponent = toPlay == PieceColor::White ? PieceColor::Black : PieceColor::White;

		auto addResult = [&](Move move) {
			if (enforceNoCheck) {
				Board newBoard = board;
				newBoard.makeMove(move);
				if (!isIllegalDueToCheck(newBoard)) {
					result.push_back(move);
				}
			}
			else {
				result.push_back(move);
			}
		};
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
					addResult(Move(root, current));
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
					addResult(Move(root, target, type));
				}
			}
			else {
				addResult(Move(root, target));
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
							addResult(Move(currentSquare, SquareIndex(i + 2 * step)));
						}
					}

					if (currentSquare.getFile() != 0) {
						nextSquare = SquareIndex(i + step - 1);
						if (board.enPassantTarget_ == nextSquare || getPieceColor(board.at(nextSquare)) == opponent) {
							maybeWithPromotion(currentSquare, nextSquare);
						}
					}
					if (currentSquare.getFile() != 7) {
						nextSquare = SquareIndex(i + step + 1);
						if (board.enPassantTarget_ == nextSquare || getPieceColor(board.at(nextSquare)) == opponent) {
							maybeWithPromotion(currentSquare, nextSquare);
						}
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
						addResult(Move("e1g1"));
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
						addResult(Move("e1c1"));
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
						addResult(Move("e8g8"));
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
						addResult(Move("e8c8"));
					}
				}
			}
		}

		return result;
	}

	export /* constexpr */ std::vector<Move> availableMoves(Board const& board) {
		std::vector<Move> opponentMoves = availableMovesRaw(board, board.whiteToPlay_ ? PieceColor::Black : PieceColor::White, {}, false);
		std::vector<SquareIndex> opponentTargets;
		for (auto const& move : opponentMoves) {
			opponentTargets.push_back(move.to_);
		}

		return availableMovesRaw(board, board.whiteToPlay_ ? PieceColor::White : PieceColor::Black, opponentTargets);
	}

	export bool isIllegalDueToCheck(Board const& board) {
		auto scan = [&](SquareIndex root, std::pair<char, char> step, char maxDist = 8) {
			auto& xStep = step.first;
			auto& yStep = step.second;
			SquareIndex current = root;
			char rawOffset = xStep + 8 * yStep;
			char dist = 0;
			while (!(dist >= maxDist
				|| (xStep + current.getFile() < 0)
				|| (xStep + current.getFile() > 7)
				|| (current.getRank() - yStep < 0)
				|| (current.getRank() - yStep > 7))) {
				current = SquareIndex(current.getRawIndex() + rawOffset);
				dist++;
				if (board.at(current) != Piece::PieceNone) {
					return std::pair(board.at(current), dist);
				}
			}
			return std::pair(Piece::PieceNone, (char)0);
		};

		SquareIndex kingPosition = board.whiteToPlay_ ? board.blackKing_ : board.whiteKing_;
		PieceColor attackingColor = board.whiteToPlay_ ? PieceColor::White : PieceColor::Black;
		char pawnAttackDirection = board.whiteToPlay_ ? 1 : -1;

		// Diagonal attackers
		for (auto const& dir : diagonal) {
			auto [ attacker, dist ] = scan(kingPosition, dir);
			if (getPieceColor(attacker) == attackingColor &&
				(getPieceType(attacker) == PieceType::Bishop || getPieceType(attacker) == PieceType::Queen
					|| (getPieceType(attacker) == PieceType::King && dist == 1)
					|| (getPieceType(attacker) == PieceType::Pawn && dist == 1 && dir.second == pawnAttackDirection))) {
				return true;
			}
		}
		for (auto const& dir : straight) {
			auto [attacker, dist] = scan(kingPosition, dir);
			if (getPieceColor(attacker) == attackingColor &&
				(getPieceType(attacker) == PieceType::Rook || getPieceType(attacker) == PieceType::Queen
					|| (getPieceType(attacker) == PieceType::King && dist == 1))) {
				return true;
			}
		}for (auto const& dir : knight) {
			auto [attacker, dist] = scan(kingPosition, dir, 1);
			if (getPieceColor(attacker) == attackingColor &&
				(getPieceType(attacker) == PieceType::Knight)) {
				return true;
			}
		}
		
		return false;
	}

} // namespace
