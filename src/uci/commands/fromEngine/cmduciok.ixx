export module cherry.uci.cmd.uciok;

import std;

export namespace cherry::uci::command {

	class UCIOk {
	public:
		explicit UCIOk() {
		}
	};

	std::ostream& operator<<(std::ostream& os, UCIOk const& /* cmd */) {
		os << "uciok";
		return os;
	}

} // namespace cherry::uci::command
