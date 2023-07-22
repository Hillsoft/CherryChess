export module cherry.uci.commandEmitter;

import std;

export namespace cherry::uci {

	class CommandEmitter {
	public:
		void emitCommand(auto const& command) {
			std::cout << command << std::endl;
		}
	};

} // namespace cherry
