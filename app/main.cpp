#include <cxxopts.hpp>
#include <curl/curl.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <vector>

#include <cstdio>
#include <cstdlib>

#include <windows.h>

namespace
{
	size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		size_t written = std::fwrite(ptr, size, nmemb, reinterpret_cast<std::FILE*>(stream));
		return written;
	}

	bool download_data(const std::string& year, const std::string& day, const std::filesystem::path& path, const std::string& cookie)
	{
		CURL* const curl = curl_easy_init();
		if (!curl)
		{
			std::cerr << "Failed to create curl handle" << std::endl;
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
			std::fclose(file);
			return result == 0;
		}
		return false;
	}
}

int main(int argc, char** argv)
{
	char* exe_path_c_str;
	if (const errno_t error = _get_pgmptr(&exe_path_c_str))
	{
		std::cerr << "failed to get the path of the executable" << std::endl;
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
	cxxopts::ParseResult result;
	try
	{
		result = options.parse(argc, argv);
	}
	catch (const cxxopts::exceptions::parsing& ex)
	{
		std::cout << options.help() << std::endl;
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	if (result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return 0;
	}

	if (!result.count("year") || !result.count("day"))
	{
		std::cerr << "expected year and day to be specified" << std::endl;
		return 0;
	}

	const auto year = std::to_string(result["year"].as<int>());
	const std::filesystem::path parent = exe_path.parent_path();
	const std::filesystem::path year_directory = parent / year;
	const auto filename = std::format("day{:02}.dll", result["day"].as<int>());
	const std::filesystem::path dll_path = year_directory / filename;
	if (!std::filesystem::exists(dll_path))
	{
		std::cerr << "Failed to find " << dll_path << std::endl;
		return ERROR_FILE_NOT_FOUND;
	}

	const HMODULE library = ::LoadLibraryW(dll_path.c_str());
	if (library == NULL)
	{
		std::cerr << "Failed to open module" << std::endl;
		return GetLastError();
	}

	if (result.count("run-tests"))
	{
		using Test_fn = bool (*)(int, char**);
		const auto test_fn = reinterpret_cast<Test_fn>(::GetProcAddress(library, "test"));
		if (!test_fn)
		{
			std::cerr << "Failed to load 'test' function from module" << std::endl;
			return GetLastError();
		}
		return test_fn(1, &exe_path_c_str) == 0;
	}


	using Solve_fn = void (*)(std::istream&);
	const auto solve_fn = reinterpret_cast<Solve_fn>(::GetProcAddress(library, "solve"));
	if (!solve_fn)
	{
		std::cerr << "Failed to load 'solve' function from module" << std::endl;
		return GetLastError();
	}

	std::istream* p_is = &std::cin;
	std::ifstream file;
	if (result.count("data-dir"))
	{
		const std::filesystem::path data_directory(result["data-dir"].as<std::string>());
		const auto day = std::to_string(result["day"].as<int>());
		const std::filesystem::path data_file = data_directory / year / "day" / day / "input.txt";
		if (!std::filesystem::exists(data_file))
		{
			if (result.count("session"))
			{
				std::cout << "The data file does not exist, trying to download" << std::endl;
				const std::string cookie = "session=" + result["session"].as<std::string>();
				std::filesystem::create_directory(data_file.parent_path());
				if (!download_data(year, day, data_file, cookie))
				{
					std::cerr << "Failed to download data" << std::endl;
					return EXIT_FAILURE;
				}
			}
			else
			{
				std::cerr << "The data file does not exist " << data_file << std::endl;
				return EXIT_FAILURE;
			}
		}
		file.open(data_file);
		p_is = &file;
	}

	const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	solve_fn(*p_is);
	const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	const std::chrono::steady_clock::duration duration = end - start;
	std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << std::endl;

	return 0;
}
