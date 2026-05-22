#include "wrap_internal.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <vector>
#include <cstring>

struct CStrEql {
	bool operator()(char const* a, char const* b) const {
		return std::strcmp(a, b) == 0;
	}
};
struct CStrHash {
	size_t operator()(char const* text) const {
		return std::hash<std::string_view>{}(text);
	}
};

bool areRequirementsSatisfied(
	std::span<char const*> required, std::span<char const*> available) {
	auto comparator = CStrEql{};

	for (auto const& requiredExample : required) {
		bool matched = false;
		for (auto const& availableExample : available) {
			if (comparator(requiredExample, availableExample)) {
				matched = true;
				continue;
			}
		}
		if (!matched) return false;
	}

	return true;
}
