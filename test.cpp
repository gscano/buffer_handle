#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <buffer_handle/boolean.hpp>
#include <buffer_handle/character.hpp>
#include <buffer_handle/string.hpp>
#include <buffer_handle/token.hpp>

using namespace buffer_handle;

SCENARIO("Character", "[character]")
{
  char buffer[1] = {0};
  char * begin = buffer;
  char * end = nullptr;

  REQUIRE((std::size_t)character<config::static_, action::size>(nullptr, 0) == 1);

  end = character<config::static_, action::prepare>(begin, 'c');

  REQUIRE(std::string(begin, end) == "c");
}

SCENARIO("String", "[string]")
{
  const std::size_t length = 32;
  char buffer[length] = {0};
  char * begin = buffer;
  char * end = buffer;

  const char pad = ' ';

  const char * data = "Hello world!";

  std::size_t max_length = 0;

  GIVEN("A buffer")
    {
      WHEN("Static")
	{
	  REQUIRE(((std::size_t)string<config::static_, align::left, pad, action::size>(nullptr, data, std::strlen(data), max_length) == std::strlen(data)));

	  end = string<config::static_, align::left, pad, action::prepare>(begin, data, std::strlen(data), max_length);

	  REQUIRE(end - begin == std::strlen(data));
	  REQUIRE(max_length == std::strlen(data));
	  REQUIRE(std::string(begin, end) == data);
	}

      WHEN("Dynamic")
	{
	  WHEN("Right-aligned")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::right, pad, action::size>(nullptr, nullptr, length, max_length);

	      REQUIRE(size == length);
	      REQUIRE(max_length == 0);

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = string<config::dynamic, align::right, pad, action::prepare>(begin, nullptr, length, max_length);

		      REQUIRE(end - begin == length);
		      REQUIRE(max_length == length);
		      REQUIRE(std::string(begin, end) == std::string(max_length, pad));

		      THEN("Write")
			{
			  end = string<config::dynamic, align::right, pad, action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == length);
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, end - std::strlen(data)) == std::string(max_length - std::strlen(data), pad));
			  REQUIRE(std::string(end - std::strlen(data), end) == data);

			  THEN("Reset")
			    {
			      end = string<config::dynamic, align::right, pad, action::reset>(begin, nullptr, 0, max_length);

			      REQUIRE(max_length == length);
			      REQUIRE(end - begin == max_length);
			      REQUIRE(std::string(begin, end) == std::string(max_length, pad));
			    }
			}
		    }
		}
	    }

	  WHEN("Left-aligned")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::left, pad, action::size>(nullptr, nullptr, length, max_length);

	      REQUIRE(size == length);
	      REQUIRE(max_length == 0);

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = string<config::dynamic, align::left, pad, action::prepare>(begin, nullptr, length, max_length);

		      REQUIRE(end - begin == length);
		      REQUIRE(max_length == length);
		      REQUIRE(std::string(begin, end) == std::string(max_length, pad));

		      THEN("Write")
			{
			  end = string<config::dynamic, align::left, pad, action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == length);
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, begin + std::strlen(data)) == data);
			  REQUIRE(std::string(begin + std::strlen(data), end) == std::string(max_length - std::strlen(data), pad));

			  THEN("Reset")
			    {
			      end = string<config::dynamic, align::left, pad, action::reset>(begin, nullptr, 0, max_length);

			      REQUIRE(max_length == length);
			      REQUIRE(end - begin == max_length);
			      REQUIRE(std::string(begin, end) == std::string(max_length, pad));
			    }
			}
		    }
		}
	    }
	}
    }
}

SCENARIO("Boolean", "[boolean]")
{
  const std::size_t length = 8;
  char buffer[length] = {0};
  char * begin = buffer;
  char * end = buffer;

  const char pad = ' ';

  GIVEN("A buffer")
    {
      WHEN("Static")
	{
	  REQUIRE(((std::size_t)boolean<config::static_, case_::lower, align::left, pad, action::size>(nullptr, true) == 4));

	  end = boolean<config::static_, case_::lower, align::left, pad, action::prepare>(begin, true);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "true");
	}

      WHEN("Dynamic")
	{
	  GIVEN("Size")
	    {
	      REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::left, pad, action::size>(nullptr, true) == 5));
	      REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::right, pad, action::size>(nullptr, true) == 5));

	      WHEN("Left-aligned")
		{
		  THEN("Prepare or write or reset")
		    {
		      end = boolean<config::dynamic, case_::lower, align::left, pad, action::prepare>(begin, true);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "true ");

		      end = boolean<config::dynamic, case_::lower, align::left, pad, action::prepare>(begin, false);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "false");
		    }
		}

	      WHEN("Right-aligned")
		{
		  THEN("Prepare or write or reset")
		    {
		      end = boolean<config::dynamic, case_::lower, align::right, pad, action::prepare>(begin, true);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == " true");

		      end = boolean<config::dynamic, case_::lower, align::right, pad, action::prepare>(begin, false);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "false");
		    }
		}
	    }
	}
    }
}
