#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <buffer_handle/boolean.hpp>
#include <buffer_handle/character.hpp>
#include <buffer_handle/number.hpp>
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
	  REQUIRE(((std::size_t)string<config::static_, align::left, ' ', action::size>(nullptr, data, std::strlen(data), max_length) == std::strlen(data)));

	  end = string<config::static_, align::left, ' ', action::prepare>(begin, data, std::strlen(data), max_length);

	  REQUIRE(end - begin == std::strlen(data));
	  REQUIRE(max_length == std::strlen(data));
	  REQUIRE(std::string(begin, end) == data);
	}

      WHEN("Dynamic")
	{
	  WHEN("Right-aligned")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::right, ' ', action::size>(nullptr, nullptr, length, max_length);

	      REQUIRE(size == length);
	      REQUIRE(max_length == 0);

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = string<config::dynamic, align::right, ' ', action::prepare>(begin, nullptr, length, max_length);

		      REQUIRE(end - begin == length);
		      REQUIRE(max_length == length);
		      REQUIRE(std::string(begin, end) == std::string(max_length, pad));

		      THEN("Write")
			{
			  end = string<config::dynamic, align::right, ' ', action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == length);
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, end - std::strlen(data)) == std::string(max_length - std::strlen(data), pad));
			  REQUIRE(std::string(end - std::strlen(data), end) == data);

			  THEN("Reset")
			    {
			      end = string<config::dynamic, align::right, ' ', action::reset>(begin, nullptr, 0, max_length);

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
	      std::size_t size = (std::size_t)string<config::dynamic, align::left, ' ', action::size>(nullptr, nullptr, length, max_length);

	      REQUIRE(size == length);
	      REQUIRE(max_length == 0);

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = string<config::dynamic, align::left, ' ', action::prepare>(begin, nullptr, length, max_length);

		      REQUIRE(end - begin == length);
		      REQUIRE(max_length == length);
		      REQUIRE(std::string(begin, end) == std::string(max_length, pad));

		      THEN("Write")
			{
			  end = string<config::dynamic, align::left, ' ', action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == length);
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, begin + std::strlen(data)) == data);
			  REQUIRE(std::string(begin + std::strlen(data), end) == std::string(max_length - std::strlen(data), pad));

			  THEN("Reset")
			    {
			      end = string<config::dynamic, align::left, ' ', action::reset>(begin, nullptr, 0, max_length);

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

  GIVEN("A buffer")
    {
      WHEN("Static")
	{
	  REQUIRE(((std::size_t)boolean<config::static_, case_::lower, align::left, ' ', action::size>(nullptr, true) == 4));

	  end = boolean<config::static_, case_::lower, align::left, ' ', action::prepare>(begin, true);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "true");
	}

      WHEN("Dynamic")
	{
	  GIVEN("Size")
	    {
	      REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::left, ' ', action::size>(nullptr, true) == 5));
	      REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::right, ' ', action::size>(nullptr, true) == 5));

	      WHEN("Left-aligned")
		{
		  THEN("Prepare or write or reset")
		    {
		      end = boolean<config::dynamic, case_::lower, align::left, ' ', action::prepare>(begin, true);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "true ");

		      end = boolean<config::dynamic, case_::lower, align::left, ' ', action::prepare>(begin, false);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "false");
		    }
		}

	      WHEN("Right-aligned")
		{
		  THEN("Prepare or write or reset")
		    {
		      end = boolean<config::dynamic, case_::lower, align::right, ' ', action::prepare>(begin, true);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == " true");

		      end = boolean<config::dynamic, case_::lower, align::right, ' ', action::prepare>(begin, false);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "false");
		    }
		}
	    }
	}
    }
}

#include <buffer_handle/adapter/itoa.hpp>

SCENARIO("Number", "[Number]")
{
  {
    REQUIRE(digits<uint8_t>(0) == 1);
    REQUIRE(digits<uint8_t>(1) == 1);
    REQUIRE(digits<uint8_t>(4) == 1);
    REQUIRE(digits<uint8_t>(67) == 2);
    REQUIRE(digits<uint8_t>(234) == 3);

    REQUIRE(digits<int>(-1) == 2);
    REQUIRE(digits<int>(-4) == 2);
    REQUIRE(digits<int>(-34) == 3);
  }

  char buffer[32] = {0};
  char * begin = buffer;
  char * end = buffer;

  GIVEN("A buffer")
    {
      uint8_t max_digits = 0;

      WHEN("Static")
	{
	  REQUIRE(((std::size_t)integral_number<config::static_, align::right, ' ', adapter::itoa, action::size>(nullptr, 1998, max_digits) == 4));

	  end = integral_number<config::static_, align::right, ' ', adapter::itoa, action::prepare>(begin, 1998, max_digits);

	  REQUIRE(end == begin + 4);
	  REQUIRE(max_digits == 4);
	  REQUIRE(std::string(begin, end) == "1998");
	}

      WHEN("Dynamic")
	{
	  WHEN("Right-aligned")
	    {
	      REQUIRE(((std::size_t)integral_number<config::dynamic, align::right, ' ', adapter::itoa, action::size>(nullptr, 19237840, max_digits) == 8));

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = integral_number<config::dynamic, align::right, ' ', adapter::itoa, action::prepare>(begin, 19237840, max_digits);
		      REQUIRE(end - begin == 8);
		      REQUIRE(max_digits == 8);
		      REQUIRE(std::string(begin, end) == std::string(8, ' '));

		      THEN("Write")
			{
			  end = integral_number<config::dynamic, align::right, ' ', adapter::itoa, action::write>(begin, 1923784, max_digits);
			  REQUIRE(end - begin == 8);
			  REQUIRE(max_digits == 8);
			  REQUIRE(std::string(begin, end) == " 1923784");

			  THEN("Reset")
			    {
			      end = integral_number<config::dynamic, align::right, ' ', adapter::itoa, action::reset>(begin, 1923784, max_digits);
			      REQUIRE(end - begin == 8);
			      REQUIRE(max_digits == 8);
			      REQUIRE(std::string(begin, end) == std::string(8, ' '));
			    }
			}
		    }
		}
	    }

	  WHEN("Left-aligned")
	    {
	      REQUIRE(((std::size_t)integral_number<config::dynamic, align::left, ' ', adapter::itoa, action::size>(nullptr, 19237840, max_digits) == 8));

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = integral_number<config::dynamic, align::left, ' ', adapter::itoa, action::prepare>(begin, 19237840, max_digits);
		      REQUIRE(end - begin == 8);
		      REQUIRE(max_digits == 8);
		      REQUIRE(std::string(begin, end) == std::string(8, ' '));

		      THEN("Write")
			{
			  end = integral_number<config::dynamic, align::left, ' ', adapter::itoa, action::write>(begin, 1923784, max_digits);
			  REQUIRE(end - begin == 8);
			  REQUIRE(max_digits == 8);
			  REQUIRE(std::string(begin, end) == "1923784 ");

			  THEN("Reset")
			    {
			      end = integral_number<config::dynamic, align::left, ' ', adapter::itoa, action::reset>(begin, 1923784, max_digits);
			      REQUIRE(end - begin == 8);
			      REQUIRE(max_digits == 8);
			      REQUIRE(std::string(begin, end) == std::string(8, ' '));
			    }
			}
		    }
		}
	    }
	}
    }
}
