export module cherry.uci.cmd.id;

import std;

export namespace cherry::uci::command {

	class UCIId {
	public:
		explicit UCIId(std::string field, std::string name)
			: field_(std::move(field)),
			name_(std::move(name)) {
		}

		std::string field_;
		std::string name_;
	};

	std::ostream& operator<<(std::ostream& os, UCIId const& cmd) {
		os << "id " << cmd.field_ << " " << cmd.name_;
		return os;
	}

} // namespace cherry::uci::command
