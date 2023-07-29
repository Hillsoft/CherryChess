import std;
import test;

import cherry.squareIndex;

cherry::test::TestP<std::string> SquareIndexParsing("SquareIndexParsing", [](cherry::test::TestRunner& runner, std::string const& code) {
	runner.expectEq<std::string_view>(code, cherry::SquareIndex(code).getCode());
	},
	{
		{"a1", "a1"},
		{"c3", "c3"},
		{"e1", "e1"},
		{"h8", "h8"}
	});

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}
