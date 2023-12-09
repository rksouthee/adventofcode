#include <catch2/catch_session.hpp>

extern "C" int test(int argc, char** argv)
{
	return Catch::Session().run(argc, argv);
}
