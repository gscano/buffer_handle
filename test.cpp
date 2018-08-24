#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <buffer_handle/character.hpp>

using namespace buffer_handle;

SCENARIO("character", "[character]")
{
  char buffer[1024] = {0};
  char * begin = buffer;
  char * end = nullptr;

  REQUIRE((std::size_t)character<config::static_, action::size>(nullptr, 0) == 1);

  end = character<config::static_, action::prepare>(begin, 'c');

  REQUIRE(std::string(begin, end) == "c");
}
