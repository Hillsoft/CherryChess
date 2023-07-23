export module cherry.evaluation;

import std;

export namespace cherry {

	class Evaluation {
	public:
		struct CPTag {};
		struct MateTag {};

		constexpr Evaluation(CPTag /* tag */, short value)
			: value_(value), myMate_(false), isMate_(false) {}

		constexpr Evaluation(MateTag /* tag */, bool myMate, short distance)
			: value_(distance), myMate_(myMate), isMate_(true) {}

		constexpr auto operator<=>(Evaluation const& other) const {
			if (isMate_ && !myMate_) {
				if (other.isMate_ && !other.myMate_) {
					return value_ - other.value_;
				}
				return -1;
			}
			if (isMate_) {
				if (other.isMate_ && other.myMate_) {
					return other.value_ - value_;
				}
				return 1;
			}
			if (other.isMate_) {
				return other.myMate_ ? -1 : 1;
			}
			return value_ - other.value_;
		}

		void step() {
			if (isMate_) {
				if (!myMate_) {
					value_++;
				}
				myMate_ = !myMate_;
			}
			else {
				value_ = -value_;
			}
		}

		void unstep() {
			if (isMate_) {
				if (myMate_) {
					value_--;
				}
				myMate_ = !myMate_;
			}
			else {
				value_ = -value_;
			}
		}

		bool isMate_;
		bool myMate_;
		short value_;
	};

	Evaluation step(Evaluation eval) {
		eval.step();
		return eval;
	}

	Evaluation unstep(Evaluation eval) {
		eval.unstep();
		return eval;
	}

	constexpr Evaluation worstEval(Evaluation::MateTag(), false, -1000);
	constexpr Evaluation bestEval(Evaluation::MateTag(), true, -1000);

	std::ostream& operator<<(std::ostream& os, Evaluation const& eval) {
		if (eval.isMate_) {
			os << "mate ";
			if (!eval.myMate_)
				os << "-";
			os << eval.value_;
		}
		else {
			os << "cp " << eval.value_;
		}
		return os;
	}

} // namespace cherry
