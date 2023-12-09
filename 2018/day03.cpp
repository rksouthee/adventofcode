#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

namespace
{
	struct Claim
	{
		int id;
		int x;
		int y;
		int w;
		int h;
	};

	Claim parse_claim(const std::string& line)
	{
		static const std::regex re("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
		if (std::smatch match; std::regex_match(line, match, re))
		{
			const int id = std::stoi(match[1].str());
			const int x = std::stoi(match[2].str());
			const int y = std::stoi(match[3].str());
			const int w = std::stoi(match[4].str());
			const int h = std::stoi(match[5].str());
			return { id, x, y, w, h };
		}
		return {};
	}

	std::vector<Claim> read_input(std::istream& is)
	{
		std::vector<Claim> claims;
		std::string line;
		while (std::getline(is, line))
		{
			claims.push_back(parse_claim(line));
		}
		return claims;
	}

	std::pair<int, int> get_max_dimensions(const std::vector<Claim>& claims)
	{
		std::pair<int, int> p{0, 0};
		for (const Claim& claim : claims)
		{
			p.first = std::max(p.first, claim.x + claim.w);
			p.second = std::max(p.second, claim.y + claim.h);
		}
		return p;
	}

	void mark_claim(std::vector<int>& fabric, const Claim& claim, const int w)
	{
		for (int y = claim.y; y < claim.y + claim.h; ++y)
		{
			for (int x = claim.x; x < claim.x + claim.w; ++x)
			{
				++fabric[y * w + x];
			}
		}
	}

	std::vector<int> make_fabric(const std::vector<Claim>& claims, const int w, const int h)
	{
		std::vector<int> fabric(w * h, 0);
		for (const Claim& claim : claims)
		{
			mark_claim(fabric, claim, w);
		}
		return fabric;
	}

	bool is_overlapping(const std::vector<int>& fabric, const int w, const Claim& claim)
	{
		for (int y = claim.y; y < claim.y + claim.h; ++y)
		{
			for (int x = claim.x; x < claim.x + claim.w; ++x)
			{
				if (fabric[y * w + x] > 1)
				{
					return true;
				}
			}
		}
		return false;
	}

	std::ptrdiff_t part_one(const std::vector<int>& fabric)
	{
		return std::count_if(fabric.begin(), fabric.end(), [] (const int c) -> bool { return c >= 2; });
	}

	int part_two(const std::vector<int>& fabric, const std::pair<int, int> dimensions, const std::vector<Claim>& claims)
	{
		for (const Claim& claim : claims)
		{
			if (!is_overlapping(fabric, dimensions.first, claim))
			{
				return claim.id;
			}
		}
		return 0;
	}
}

extern "C" void solve(std::istream& is)
{
	const std::vector<Claim> claims = read_input(is);
	const std::pair<int, int> dimensions = get_max_dimensions(claims);
	const std::vector<int> fabric = make_fabric(claims, dimensions.first, dimensions.second);

	std::cout << part_one(fabric) << std::endl;
	std::cout << part_two(fabric, dimensions, claims) << std::endl;
}
