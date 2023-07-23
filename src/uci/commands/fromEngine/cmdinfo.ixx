export module cherry.uci.cmd.info;

import std;

export namespace cherry::uci::command {

	class UCIInfo {
	public:
		explicit UCIInfo(std::optional<int> score)
			: score_(std::move(score)) {
		}

		std::optional<int> score_;
	};

	std::ostream& operator<<(std::ostream& os, UCIInfo const& cmd) {
		if (cmd.score_.has_value()) {
			os << "info score cp " << *cmd.score_;
		}
		return os;
	}

} // namespace cherry::uci::command
