export module cherry.searchResult;

import cherry.evaluationRange;
import cherry.move;

export namespace cherry {

	class SearchResult {
	public:
		constexpr SearchResult()
			: depth_(-1) {}

		constexpr SearchResult(int depth, EvaluationRange eval, Move move)
			: depth_(depth), eval_(eval), move_(move) {}

		constexpr SearchResult(SearchResult const& other) = default;
		constexpr SearchResult(SearchResult&& other) = default;

		constexpr SearchResult& operator=(SearchResult const& other) = default;
		constexpr SearchResult& operator=(SearchResult&& other) = default;

		int depth_;
		EvaluationRange eval_;
		Move move_;
	};

} // namespace cherry
