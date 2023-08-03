export module cherry.searchResult;

import cherry.evaluationRange;
import cherry.move;

export namespace cherry {

	class SearchResult {
	public:
		SearchResult()
			: depth_(-1) {}

		SearchResult(int depth, EvaluationRange eval, Move move)
			: depth_(depth), eval_(eval), move_(move) {}

		int depth_;
		EvaluationRange eval_;
		Move move_;
	};

} // namespace cherry
