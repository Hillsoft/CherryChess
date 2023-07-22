export module cherry.uci.cmd.readyok;

import std;

export namespace cherry::uci::command {

	class ReadyOk {
	public:
		explicit ReadyOk() {
		}
	};

	std::ostream& operator<<(std::ostream& os, ReadyOk const& /* cmd */) {
		os << "readyok";
		return os;
	}

} // namespace cherry::uci::command
