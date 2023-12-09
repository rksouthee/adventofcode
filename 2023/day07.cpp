#include <aoc/algorithm.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <cassert>

namespace
{
	enum class Card
	{
		joker = 1,
		two = 2,
		three = 3,
		four = 4,
		five = 5,
		six = 6,
		seven = 7,
		eight = 8,
		nine = 9,
		ten = 10,
		jack = 11,
		queen = 12,
		king = 13,
		ace = 14
	};

	Card from_char(const char ch, const bool j_is_joker)
	{
		switch (ch)
		{
		case '2': return Card::two;
		case '3': return Card::three;
		case '4': return Card::four;
		case '5': return Card::five;
		case '6': return Card::six;
		case '7': return Card::seven;
		case '8': return Card::eight;
		case '9': return Card::nine;
		case 'T': return Card::ten;
		case 'J': return j_is_joker ? Card::joker : Card::jack;
		case 'Q': return Card::queen;
		case 'K': return Card::king;
		case 'A': return Card::ace;
		}
		return Card::two;
	}

	using Hand = std::array<Card, 5>;

	Hand make_hand(const std::string_view str, const bool j_is_joker)
	{
		assert(str.size() == 5);
		Hand hand;
		std::transform(str.begin(), str.end(), hand.begin(), [j_is_joker] (const char ch) { return from_char(ch, j_is_joker); });
		return hand;
	}

	enum class Hand_rank
	{
		high_card,
		one_pair,
		two_pair,
		three_of_a_kind,
		full_house,
		four_of_a_kind,
		five_of_a_kind,
	};

	using Card_count = std::pair<Card, int>;

	bool is_five_of_a_kind(const std::vector<Card_count>& hand)
	{
		return hand.size() == 1 && hand.back().second == 5;
	}

	bool is_four_of_a_kind(const std::vector<Card_count>& hand)
	{
		return hand.size() == 2 && hand.back().second == 4;
	}

	bool is_full_house(const std::vector<Card_count>& hand)
	{
		return hand.size() == 2 && hand.front().second == 2 && hand.back().second == 3;
	}

	bool is_three_of_a_kind(const std::vector<Card_count>& hand)
	{
		return hand.size() == 3 && hand.back().second == 3;
	}

	bool is_two_pair(const std::vector<Card_count>& hand)
	{
		return hand.size() == 3 && hand[1].second == 2 && hand[2].second == 2;
	}

	bool is_one_pair(const std::vector<Card_count>& hand)
	{
		return hand.size() == 4 && hand.back().second == 2;
	}

	std::vector<Card_count> card_count(Hand hand)
	{
		std::sort(hand.begin(), hand.end());
		std::vector<Card_count> counts;
		aoc::unique_copy_with_count(hand.begin(), hand.end(), std::back_inserter(counts));
		std::sort(counts.begin(), counts.end(), [] (const Card_count& x, const Card_count& y) -> bool
			  {
				  return x.second < y.second;
			  });
		return counts;
	}

	Hand_rank get_hand_rank(const Hand& hand)
	{
		const std::vector<Card_count> counts = card_count(hand);
		if (is_five_of_a_kind(counts))	return Hand_rank::five_of_a_kind;
		if (is_four_of_a_kind(counts))	return Hand_rank::four_of_a_kind;
		if (is_full_house(counts))	return Hand_rank::full_house;
		if (is_three_of_a_kind(counts))	return Hand_rank::three_of_a_kind;
		if (is_two_pair(counts))	return Hand_rank::two_pair;
		if (is_one_pair(counts))	return Hand_rank::one_pair;
		return Hand_rank::high_card;
	}

	Hand_rank get_strongest_hand_rank(const Hand& hand)
	{
		Hand_rank strongest_rank = get_hand_rank(hand);
		for (int i = 2; i < 15; ++i)
		{
			if (i == (int)Card::jack) continue;
			const auto new_card = static_cast<Card>(i);
			Hand new_hand = hand;
			std::replace(new_hand.begin(), new_hand.end(), Card::joker, new_card);
			const Hand_rank new_rank = get_hand_rank(new_hand);
			strongest_rank = std::max(strongest_rank, new_rank);
		}
		return strongest_rank;
	}

	struct Input
	{
		std::string hand;
		int bid;
	};

	std::vector<Input> read_input(std::istream& is)
	{
		std::vector<Input> hands;
		std::string hand;
		int bid;
		while (is >> hand >> bid)
		{
			hands.emplace_back(hand, bid);
		}
		return hands;
	}

	struct Camel_hand
	{
		Hand hand;
		Hand_rank rank;
		int bid;
	};

	std::size_t get_total_winnings(const std::vector<Input>& inputs, const bool j_is_joker, Hand_rank (*ranker)(const Hand&))
	{
		std::vector<Camel_hand> hands;
		for (const Input& input : inputs)
		{
			const Hand hand = make_hand(input.hand, j_is_joker);
			const Hand_rank rank = ranker(hand);
			hands.emplace_back(hand, rank, input.bid);
		}
		std::sort(hands.begin(), hands.end(), [] (const Camel_hand& x, const Camel_hand& y) -> bool
			  {
				  return std::tie(x.rank, x.hand) < std::tie(y.rank, y.hand);
			  });
		std::size_t total_winnings = 0;
		for (std::size_t i = 0; i < hands.size(); ++i)
		{
			const std::size_t rank = i + 1;
			total_winnings += (rank * hands[i].bid);
		}
		return total_winnings;
	}

	std::size_t part_one(const std::vector<Input>& inputs)
	{
		return get_total_winnings(inputs, false, get_hand_rank);
	}

	std::size_t part_two(const std::vector<Input>& inputs)
	{
		return get_total_winnings(inputs, true, get_strongest_hand_rank);
	}
}

extern "C" void solve(std::istream& is)
{
	const std::vector<Input> inputs = read_input(is);
	std::cout << part_one(inputs) << std::endl;
	std::cout << part_two(inputs) << std::endl;
}
