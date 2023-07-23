export module cherry.uci.cmd.info;

import std;

import cherry.evaluation;

export namespace cherry::uci::command {

	class UCIInfo {
	public:
		explicit UCIInfo(std::optional<Evaluation> score)
			: score_(std::move(score)) {
		}

		std::optional<Evaluation> score_;
	};

	std::ostream& operator<<(std::ostream& os, UCIInfo const& cmd) {
		if (cmd.score_.has_value()) {
			os << "info score " << *cmd.score_;
		}
		return os;
	}

} // namespace cherry::uci::command
