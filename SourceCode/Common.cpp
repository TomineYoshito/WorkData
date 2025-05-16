#include "Common.h"

vector<string> Common::split(string str, char del) {
	size_t first = 0;
	size_t last = str.find_first_of(del);

	std::vector<std::string> result;

	while (first < str.size()) {
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del, first);

		if (last == std::string::npos) {
			last = str.size();
		}
	}

	return result;
}
