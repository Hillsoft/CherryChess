module;

#include <assert.h>

export module cherry.uci.toEngine;

import std;

export import cherry.uci.cmd.isready;
export import cherry.uci.cmd.quit;
export import cherry.uci.cmd.uci;

namespace {

	std::vector<std::string_view> tokenize(std::string_view input) {
		auto cursor = input.begin();
		std::vector<std::string_view> result;

		auto skipWhitespace = [&]() {
			while (cursor < input.end() && std::isspace(*cursor)) {
				cursor++;
			}
		};
		auto skipChars = [&]() {
			while (cursor < input.end() && !std::isspace(*cursor)) {
				cursor++;
			}
		};
		
		while (cursor < input.end()) {
			skipWhitespace();
			auto tokenStart = cursor;
			skipChars();
			if (cursor > tokenStart) {
				result.emplace_back(tokenStart, cursor);
			}
		}


		return result;
	}

} // namespace

export namespace cherry::uci {

	class ToEngineCommand {
	public:
		ToEngineCommand(auto cmd)
			: command_(std::move(cmd)) {}

		std::variant<command::IsReady, command::Quit, command::UCI> command_;
	};

	std::optional<ToEngineCommand> parseTokens(std::span<std::string_view> tokens) {
		assert(tokens.size() > 0);
		if (tokens[0] == "isready") {
			return command::IsReady(tokens.subspan(1));
		}
		if (tokens[0] == "uci") {
			return command::UCI(tokens.subspan(1));
		}
		if (tokens[0] == "quit") {
			return command::Quit(tokens.subspan(1));
		}
		return std::nullopt;
	}

	std::optional<ToEngineCommand> parseCommand(std::string_view rawCommand) {
		std::vector<std::string_view> tokens = tokenize(rawCommand);
		std::span<std::string_view> tokensRng(tokens);
		while (tokensRng.size() > 0) {
			std::optional<ToEngineCommand> result = parseTokens(tokensRng);
			if (result.has_value()) {
				return result;
			}
			tokensRng = tokensRng.subspan(1);
		}
		return std::nullopt;
	}

} // namespace cherry::uci
