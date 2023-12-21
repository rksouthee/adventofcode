#include "aoc.h"
#include <aoc/string_helpers.h>

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <cstdint>

namespace
{
	using S64 = std::int64_t;

	class Module;

	struct Pulse
	{
		Module* from;
		Module* target;
		int value;
	};

	class Module
	{
	protected:
		std::vector<Module*> m_destination;

		void send(int value, std::queue<Pulse>& queue)
		{
			for (Module* module : m_destination)
			{
				if (module) queue.emplace(this, module, value);
			}
		}

	public:
		virtual ~Module() = default;

		void add_output(Module* module)
		{
			m_destination.push_back(module);
		}

		virtual void add_input(Module*) {};

		virtual void handle(Module* from, int value, std::queue<Pulse>& queue) = 0;
	};

	class Empty : public Module
	{
	public:
		void handle(Module*, int, std::queue<Pulse>&) { }
	};

	class Broadcaster : public Module
	{
	private:
		void handle(Module*, int value, std::queue<Pulse>& queue) override
		{
			send(value, queue);
		}
	};

	class Flip_flop : public Module
	{
	private:
		bool m_on = false;

	public:
		~Flip_flop() override = default;

		void handle(Module*, int value, std::queue<Pulse>& queue) override
		{
			if (value == 1) return;
			value = m_on ? 0 : 1;
			m_on = !m_on;
			send(value, queue);
		}
	};

	class Conjunction : public Module
	{
	private:
		std::unordered_map<Module*, int> m_pulses;
		std::size_t count = 0;

	public:
		~Conjunction() override = default;

		void add_input(Module* module) override
		{
			m_pulses.insert(std::make_pair(module, 0));
		}

		void handle(Module* from, int value, std::queue<Pulse>& queue) override
		{
			int& prev = m_pulses[from];
			if (value != prev)
			{
				if (value == 0) --count;
				else ++count;
				prev = value;
			}

			if (count == m_pulses.size()) send(0, queue);
			else send(1, queue);
		}
	};

	struct Module_info
	{
		std::string name;
		std::vector<std::string> modules;
		char type;
	};

	Module_info parse_module(const std::string& line)
	{
		Module_info info;
		const std::vector<std::string_view> parts = aoc::split(line, " -> ");
		assert(parts.size() == 2);
		if (parts.front().front() == '%' || parts.front().front() == '&')
		{
			info.name = std::string(parts.front().substr(1));
			info.type = parts.front().front();
		}
		else
		{
			info.name = std::string(parts.front());
			info.type = '\0';
		}

		for (const std::string_view sv : aoc::split(parts.back(), ", "))
		{
			info.modules.push_back(std::string(sv));
		}

		return info;
	}

	std::vector<Module_info> read_lines(std::istream& is)
	{
		std::vector<Module_info> modules;
		for (std::string line; std::getline(is, line);)
		{
			modules.push_back(parse_module(line));
		}
		return modules;
	}

	std::unordered_map<std::string, std::unique_ptr<Module>> read_input(std::istream& is)
	{
		const std::vector<Module_info> lines = read_lines(is);
		std::unordered_map<std::string, std::unique_ptr<Module>> modules;

		for (const Module_info& info : lines)
		{
			assert(!info.name.empty());
			if (info.type == '%') modules[info.name] = std::make_unique<Flip_flop>();
			else if (info.type == '&') modules[info.name] = std::make_unique<Conjunction>();
			else modules[info.name] = std::make_unique<Broadcaster>();
		}

		for (const Module_info& info : lines)
		{
			Module& module = *modules[info.name];
			for (const std::string& dest : info.modules)
			{
				std::unique_ptr<Module>& dp = modules[dest];
				if (!dp) dp = std::make_unique<Empty>();
				module.add_output(dp.get());
				dp->add_input(&module);
			}
		}

		return modules;
	}

	/* std::pair<S64, S64> push_button(std::unordered_map<std::string, std::unique_ptr<Module>>& modules) */
	bool push_button(std::unordered_map<std::string, std::unique_ptr<Module>>& modules)
	{
		const Module* const rx_ptr = modules["rx"].get();
		std::pair<S64, S64> result{0, 0};
		std::queue<Pulse> queue;
		queue.emplace(modules["button"].get(), modules["broadcaster"].get(), 0);
		do
		{
			Pulse pulse = queue.front();
			if (pulse.target == rx_ptr && pulse.value == 0) return true;
			if (pulse.value == 0) ++result.first;
			else ++result.second;
			queue.pop();
			pulse.target->handle(pulse.from, pulse.value, queue);
		}
		while (!queue.empty());
		/* return result; */
		return false;
	}

	/* S64 part_one(std::unordered_map<std::string, std::unique_ptr<Module>>& modules) */
	/* { */
	/* 	std::pair<S64, S64> result; */
	/* 	for (int i = 0; i < 1000; ++i) */
	/* 	{ */
	/* 		std::pair<S64, S64> temp = push_button(modules); */
	/* 		result.first += temp.first; */
	/* 		result.second += temp.second; */
	/* 	} */
	/* 	return result.first * result.second; */
	/* } */

	S64 part_two(std::unordered_map<std::string, std::unique_ptr<Module>>& modules)
	{
		S64 counter = 0;
		while (!push_button(modules)) ++counter;
		return counter;
	}
}

SOLVE
{
	std::unordered_map<std::string, std::unique_ptr<Module>> modules = read_input(std::cin);

	std::cout << part_two(modules) << std::endl;
}
