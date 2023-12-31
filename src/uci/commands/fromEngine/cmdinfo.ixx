export module cherry.uci.cmd.info;

import std;

import cherry.evaluation;

export namespace cherry::uci::command {

	struct UCIInfoData {
		std::optional<long long> timeMs;
		std::optional<long long> nodes;
		std::optional<int> depth;
		std::optional<Evaluation> score;
	};

	class UCIInfo {
	public:
		explicit UCIInfo(UCIInfoData data)
			: data_(std::move(data)) {
		}

		UCIInfoData data_;
	};

	std::ostream& operator<<(std::ostream& os, UCIInfo const& cmd) {
		if (cmd.data_.nodes.has_value() || cmd.data_.score.has_value() || cmd.data_.timeMs.has_value() || cmd.data_.depth.has_value()) {
			os << "info";
			if (cmd.data_.timeMs.has_value()) {
				os << " time " << *cmd.data_.timeMs;
			}
			if (cmd.data_.nodes.has_value()) {
				os << " nodes " << *cmd.data_.nodes;
			}
			if (cmd.data_.timeMs.has_value() && cmd.data_.nodes.has_value()) {
				os << " nps " << 1000 * *cmd.data_.nodes / *cmd.data_.timeMs;
			}
			if (cmd.data_.score.has_value()) {
				os << " score " << *cmd.data_.score;
			}
			if (cmd.data_.depth.has_value()) {
				os << " depth " << *cmd.data_.depth;
			}
		}
		return os;
	}

} // namespace cherry::uci::command
