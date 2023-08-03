export module cherry.evaluationRange;

import std;

import cherry.evaluation;

export namespace cherry {

	class EvaluationRange {
	public:
		EvaluationRange()
			: low_(worstEval), high_(bestEval) {}

		explicit EvaluationRange(Evaluation eval)
			: low_(eval), high_(eval) {}

		EvaluationRange(Evaluation low, Evaluation high)
			: low_(low), high_(high) {}

		void join(EvaluationRange const& other) {
			low_ = std::max(low_, other.low_);
			high_ = std::min(high_, other.high_);
		}

		void step() {
			Evaluation tmp = low_;
			low_ = high_;
			high_ = tmp;
			low_.step();
			high_.step();
		}

		Evaluation low_;
		Evaluation high_;
	};

} // namespace cherry
