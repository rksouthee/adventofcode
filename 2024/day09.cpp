#include "aoc.h"
#include "aoc/algorithm.h"
#include "aoc/core.h"

#include <iostream>
#include <string>
#include <vector>

namespace
{
	S64 part_one(const std::string_view input)
	{
		std::vector<S64> file_ids;
		S64 file_id = 0;
		bool is_file = true;
		for (const char c : input)
		{
			file_ids.insert(file_ids.end(), c - '0', is_file ? file_id : -1);
			if (is_file) ++file_id;
			is_file = !is_file;
		}

		const auto p = [](const S64 id) -> bool { return id >= 0; };
		auto f = std::ranges::find_if_not(file_ids, p);
		auto l = aoc::find_if_backward(f, file_ids.end(), p);
		if (l != file_ids.begin()) --l;
		while (f != l)
		{
			*f = *l;
			do ++f; while (f != l && *f >= 0);
			if (f == l) break;
			do --l; while (f != l && *l < 0);
		}

		S64 result = 0;
		const S64 n = l - file_ids.begin();
		for (S64 i = 0; i < n; ++i)
		{
			result += i * file_ids[i];
		}

		return result;
	}
}

SOLVE
{
	std::string line;
	std::getline(std::cin, line);

	std::cout << part_one(line) << '\n';
}
