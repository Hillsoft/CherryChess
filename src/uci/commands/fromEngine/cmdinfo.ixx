export module cherry.uci.cmd.info;

import std;

import cherry.evaluation;

export namespace cherry::uci::command {

	class UCIInfo {
	public:
		explicit UCIInfo(std::optional<int> nodes, std::optional<Evaluation> score)
			: nodes_(std::move(nodes)), score_(std::move(score)) {
		}

		std::optional<int> nodes_;
		std::optional<Evaluation> score_;
	};

	std::ostream& operator<<(std::ostream& os, UCIInfo const& cmd) {
		if (cmd.nodes_.has_value() || cmd.score_.has_value()) {
			os << "info";
			if (cmd.nodes_.has_value()) {
				os << " nodes " << *cmd.nodes_;
			}
			if (cmd.score_.has_value()) {
				os << " score " << *cmd.score_;
			}
		}
		return os;
	}

} // namespace cherry::uci::command
