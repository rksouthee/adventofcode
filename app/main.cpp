#include <cxxopts.hpp>
#include <curl/curl.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>

namespace
{
	std::size_t write_data(const void* const ptr, const std::size_t size, const std::size_t count, void* stream)
	{
		const std::size_t written = std::fwrite(ptr, size, count, static_cast<std::FILE*>(stream));
		return written;
	}

	bool download_data(const std::string& year, const std::string& day, const std::filesystem::path& path, const std::string& cookie)
	{
		CURL* const curl = curl_easy_init();
		if (!curl)
		{
			std::cerr << "Failed to create curl handle\n";
			return false;
		}
		const std::string url = std::format("https://adventofcode.com/{}/day/{}/input", year, day);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
		if (std::FILE* file; _wfopen_s(&file, path.c_str(), L"wb") == 0)
		{
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
			const CURLcode result = curl_easy_perform(curl);
			return std::fclose(file) == 0 && result == 0;
		}
		return false;
	}
}

int main(int argc, char** argv)
{
	char* exe_path_c_str;
	if (const errno_t error = _get_pgmptr(&exe_path_c_str))
	{
		std::cerr << "failed to get the path of the executable\n";
		return error;
	}
	const std::filesystem::path exe_path{exe_path_c_str};

	cxxopts::Options options(exe_path.stem().string(), "Advent of Code solutions");
	options.add_options()
		("h,help", "print options")
		("y,year", "year", cxxopts::value<int>())
		("d,day", "day", cxxopts::value<int>())
		("run-tests", "run tests and quit")
		("D,data-dir", "directory containing input", cxxopts::value<std::string>())
		("v,verbose", "enable verbose logging")
		("s,session", "session used to download data", cxxopts::value<std::string>())
		;
	options.parse_positional({"year", "day"});
	cxxopts::ParseResult result;
	try
	{
		result = options.parse(argc, argv);
	}
	catch (const cxxopts::exceptions::parsing& ex)
	{
		std::cout << options.help() << '\n';
		std::cerr << ex.what() << '\n';
		return 1;
	}

	if (result.count("help"))
	{
		std::cout << options.help() << '\n';
		return 0;
	}

	if (!result.count("year") || !result.count("day"))
	{
		std::cerr << "expected year and day to be specified\n";
		return 0;
	}

	const auto year = std::to_string(result["year"].as<int>());
	const std::filesystem::path parent = exe_path.parent_path();
	const std::filesystem::path year_directory = parent / year;
	const auto filename = std::format("day{:02}.dll", result["day"].as<int>());
	const std::filesystem::path dll_path = year_directory / filename;
	if (!exists(dll_path))
	{
		std::cerr << "Failed to find " << dll_path << '\n';
		return ERROR_FILE_NOT_FOUND;
	}

	const HMODULE library = LoadLibraryW(dll_path.c_str());
	if (library == nullptr)
	{
		const DWORD error = GetLastError();
		std::cerr << "Failed to open module: " << error << '\n';
		return EXIT_FAILURE;
	}

	const std::vector<std::string>& unmatched = result.unmatched();
	const auto forward_argc = 1 + static_cast<int>(unmatched.size());
	auto** forward_argv = new char*[forward_argc + 1];
	forward_argv[0] = exe_path_c_str;
	for (std::size_t i = 0; i < unmatched.size(); ++i) forward_argv[i + 1] = _strdup(unmatched[i].c_str());
	forward_argv[forward_argc] = nullptr;

	if (result.count("run-tests"))
	{
		using Test_fn = bool (*)(int, char**);
		const auto test_fn = reinterpret_cast<Test_fn>(GetProcAddress(library, "test"));  // NOLINT(clang-diagnostic-cast-function-type-strict)
		if (!test_fn)
		{
			const DWORD error = GetLastError();
			std::cerr << "Failed to load 'test' function from module: " << error << '\n';
			return EXIT_FAILURE;
		}
		return test_fn(forward_argc, forward_argv) == 0;
	}


	using Solve_fn = void (*)(int, char**);
	const auto solve_fn = reinterpret_cast<Solve_fn>(GetProcAddress(library, "solve"));  // NOLINT(clang-diagnostic-cast-function-type-strict)
	if (!solve_fn)
	{
		const DWORD error = GetLastError();
		std::cerr << "Failed to load 'solve' function from module: " << error << '\n';
		return EXIT_FAILURE;
	}

	// ReSharper disable once CppTooWideScope (We need to keep the file in scope since we may bind std::cin to it)
	std::ifstream file;
	if (result.count("data-dir"))
	{
		const std::filesystem::path data_directory(result["data-dir"].as<std::string>());
		const auto day = std::to_string(result["day"].as<int>());
		const std::filesystem::path data_file = data_directory / year / "day" / day / "input.txt";
		if (!exists(data_file))
		{
			if (result.count("session"))
			{
				std::cout << "The data file does not exist, trying to download\n";
				const std::string cookie = "session=" + result["session"].as<std::string>();
				create_directories(data_file.parent_path());
				if (download_data(year, day, data_file, cookie))
				{
					std::clog << "Downloaded data to " << data_file << '\n';
				}
				else
				{
					std::cerr << "Failed to download data" << '\n';
					return EXIT_FAILURE;
				}
			}
			else
			{
				std::cerr << "The data file does not exist " << data_file << '\n';
				return EXIT_FAILURE;
			}
		}
		file.open(data_file);
		std::cin.rdbuf(file.rdbuf());
	}

	const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	solve_fn(forward_argc, forward_argv);
	const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	const std::chrono::steady_clock::duration duration = end - start;
	std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << '\n';

	return EXIT_SUCCESS;
}
