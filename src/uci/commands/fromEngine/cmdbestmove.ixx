export module cherry.uci.cmd.bestmove;

import std;

import cherry.move;

export namespace cherry::uci::command {

	class BestMove {
	public:
		explicit BestMove(Move move)
			: move_(move) {}

		Move move_;
	};

	std::ostream& operator<<(std::ostream& os, BestMove const& cmd) {
		os << "bestmove " << cmd.move_.getCode();
		return os;
	}

} // namespace cherry::uci::command
