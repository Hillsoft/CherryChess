export module cherry.bitwiseHash;

import std;

export namespace cherry {

	constexpr size_t bitwiseHash(char const* start, char const* end) {
		size_t seed = 0;

		auto addHash([&](auto item) {
			std::hash<decltype(item)> hasher;
			seed ^= hasher(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			});

		while ((end - start) * sizeof(char) / sizeof(size_t) > 0) {
			addHash(*(size_t*)start);
			start += sizeof(size_t) / sizeof(char);
		}
		while (start < end) {
			addHash(*start);
			start++;
		}
		return seed;
	}

} // namespace cherry
