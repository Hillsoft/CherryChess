module;

#include <assert.h>

export module cherry.inlineStack;

import std;

export namespace cherry {

	template<typename TVal, size_t maxSize>
	class InlineStack {
	public:
		struct Iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = TVal;
			using pointer = TVal*;
			using reference = TVal&;

			Iterator(TVal* ptr)
				: ptr_(ptr) {}

			TVal& operator*() const { return *ptr_; }
			TVal* operator->() { return ptr_; }

			Iterator& operator++() {
				ptr_++;
				return *this;
			}

			Iterator operator++(int) {
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}

			bool operator==(Iterator const& other) {
				return ptr_ == other.ptr_;
			}

			bool operator!=(Iterator const& other) {
				return ptr_ != other.ptr_;
			}

		private:
			TVal* ptr_;
		};

		InlineStack()
			: currentSize_(0) {}

		void push(TVal val) {
			assert(currentSize_ < maxSize);
			data_[currentSize_] = std::move(val);
			currentSize_++;
		}

		TVal&& pop() {
			assert(currentSize_ > 0);
			currentSize_--;
			return std::move(data_[currentSize_]);
		}

		size_t size() const {
			return currentSize_;
		}

		Iterator begin() {
			return Iterator(&data_[0]);
		}

		Iterator end() {
			return Iterator(&data_[currentSize_]);
		}

	private:
		std::array<TVal, maxSize> data_;
		size_t currentSize_;
	};

	template<typename TVal, size_t stackSize>
	class InlineStackWriter {
	public:
		InlineStackWriter(InlineStack<TVal, stackSize>& stack, TVal val)
			: stack_(stack) {
			stack_.push(val);
		}

		~InlineStackWriter() {
			stack_.pop();
		}

	private:
		InlineStack<TVal, stackSize>& stack_;
	};

} // namespace cherry
