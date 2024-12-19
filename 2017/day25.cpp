#include "aoc.h"
#include "aoc/core.h"

#include <iostream>
#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace
{
	struct Operation
	{
		char write;
		S64 move;
		char next_state;
	};

	struct State
	{
		std::array<Operation, 2> operations;
	};

	struct Blueprint
	{
		char begin_state;
		S64 steps;
		std::unordered_map<char, State> states;
	};

	Blueprint read_blueprint(std::istream& in)
	{
		Blueprint blueprint;
		std::string line;
		std::getline(in, line);
		blueprint.begin_state = line[line.size() - 2];
		std::getline(in, line);
		blueprint.steps = std::stoll(line.substr(36));
		while (std::getline(in, line))
		{
			if (line.empty()) continue;
			const char state = line[9];
			State& state_data = blueprint.states[state];
			for (S64 i = 0; i < 2; ++i)
			{
				std::getline(in, line);
				std::getline(in, line);
				state_data.operations[i].write = line[22];
				std::getline(in, line);
				state_data.operations[i].move = line[27] == 'r' ? 1 : -1;
				std::getline(in, line);
				state_data.operations[i].next_state = line[26];
			}
		}
		return blueprint;
	}

	S64 part_one(const Blueprint& blueprint)
	{
		std::unordered_set<S64> tape;
		S64 position = 0;
		char name = blueprint.begin_state;
		for (S64 i = 0; i < blueprint.steps; ++i)
		{
			const State& state = blueprint.states.at(name);
			const Operation& operation = tape.contains(position) ? state.operations[1] : state.operations[0];
			// write
			if (operation.write == '1')
			{
				tape.insert(position);
			}
			else
			{
				tape.erase(position);
			}
			// move
			position += operation.move;
			// next state
			name = operation.next_state;
		}
		return std::ssize(tape);
	}
}

SOLVE
{
	const Blueprint blueprint = read_blueprint(std::cin);

	std::cout << part_one(blueprint) << '\n';
}