#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <buffer_handle/boolean.hpp>
#include <buffer_handle/character.hpp>
#include <buffer_handle/container.hpp>
#include <buffer_handle/date.hpp>
#include <buffer_handle/nothing.hpp>
#include <buffer_handle/number.hpp>
#include <buffer_handle/string.hpp>
#include <buffer_handle/time.hpp>
#include <buffer_handle/timezone.hpp>
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

SCENARIO("nothing", "[nothing]")
{
  char c;

  REQUIRE(((std::size_t)nothing_t().handle<action::size>(nullptr) == 0));
  REQUIRE(nothing_t().handle<action::prepare>(&c) == &c);
  REQUIRE(nothing_t().handle<action::write>(&c) == &c);
  REQUIRE(nothing_t().handle<action::reset>(&c) == &c);
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
		  THEN("Prepare with no content")
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

		  THEN("Prepare")
		    {
		      max_length = std::strlen(data);
		      end = string<config::dynamic, align::right, ' ', action::prepare>(begin, "world!", 6, max_length);

		      REQUIRE(end - begin == std::strlen(data));
		      REQUIRE(max_length == std::strlen(data));
		      REQUIRE(std::string(begin, end) == std::string(max_length - 6, pad) + "world!");

		      THEN("Write")
			{
			  end = string<config::dynamic, align::right, ' ', action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == std::strlen(data));
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, end - std::strlen(data)) == std::string(max_length - std::strlen(data), pad));
			  REQUIRE(std::string(end - std::strlen(data), end) == data);
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
		  THEN("Prepare with no content")
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

		  THEN("Prepare")
		    {
		      max_length = std::strlen(data);
		      end = string<config::dynamic, align::left, ' ', action::prepare>(begin, "world!", 6, max_length);

		      REQUIRE(end - begin == std::strlen(data));
		      REQUIRE(max_length == std::strlen(data));
		      REQUIRE(std::string(begin, end) == std::string("world!") + std::string(max_length - 6, pad));

		      THEN("Write")
			{
			  end = string<config::dynamic, align::left, ' ', action::write>(begin, data, std::strlen(data), max_length);

			  REQUIRE(max_length == std::strlen(data));
			  REQUIRE(end - begin == max_length);
			  REQUIRE(std::string(begin, end - std::strlen(data)) == std::string(max_length - std::strlen(data), pad));
			  REQUIRE(std::string(end - std::strlen(data), end) == data);
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
      WHEN("Two digits number")
	{
	  end = two_digits_number<config::static_, false, action::prepare, uint8_t>(begin, 43);

	  REQUIRE(end - begin == 2);
	  REQUIRE(std::string(begin, end) == "43");

	  end = two_digits_number<config::static_, false, action::prepare, uint8_t>(begin, 7);

	  REQUIRE(end - begin == 2);
	  REQUIRE(std::string(begin, end) == "07");

	  end = two_digits_number<config::static_, ' ', action::prepare, uint8_t>(begin, 7);

	  REQUIRE(end - begin == 2);
	  REQUIRE(std::string(begin, end) == " 7");
	}

      WHEN("Four digits number")
	{
	  end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 2);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "0002");

	  end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 56);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "0056");

	  end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 723);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "0723");

	  end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 2968);

	  REQUIRE(end - begin == 4);
	  REQUIRE(std::string(begin, end) == "2968");
	}

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

SCENARIO("Time", "[time]")
{
  const char pad = ' ';

  const std::size_t size = 8;
  char buffer[8] = {0};
  char * begin = buffer;
  char * end = buffer;

  GIVEN("A buffer")
    {
      WHEN("Using 'time_t'")
	{
	  time_t time = 193749;

	  uint8_t max_digits = 0;

	  REQUIRE(((std::size_t)time_<config::dynamic, align::right, pad, adapter::itoa, action::size>(nullptr, time, max_digits) == 6));
	}

      WHEN("Specfic")
	{
	  uint8_t hours = 5;
	  uint8_t minutes = 23;
	  uint8_t seconds = 59;

	  REQUIRE(((std::size_t)time_<config::static_, action::size>(nullptr, hours, minutes, seconds) == size));

	  GIVEN("Size")
	    {
	      THEN("Prepare")
		{
		  end = time_<config::static_, action::prepare>(buffer, hours, minutes, seconds);

		  REQUIRE(end - begin == size);
		  REQUIRE(std::string(begin, end) == "05:23:59");
		}
	    }
	}

      WHEN("Using 'struct tm'")
	{
	  struct tm time;
	  time.tm_hour = 4;
	  time.tm_min = 34;
	  time.tm_sec = 0;

	  REQUIRE(((std::size_t)time_<config::static_, action::size>(nullptr, time) == size));

	  GIVEN("Size")
	    {
	      THEN("Prepare")
		{
		  end = time_<config::static_, action::prepare>(buffer, time);

		  REQUIRE(end - begin == size);
		  REQUIRE(std::string(begin, end) == "04:34:00");
		}
	    }
	}
    }
}

SCENARIO("Timezone", "[timezone]")
{
#define TST(C, I)							\
  REQUIRE(military_timezone_offset_2_letter<int8_t>((int8_t)(I)) == (C)); \
  REQUIRE(military_timezone_letter_2_offset(C) == (I))

  TST('Z', 0);

  TST('A', 1); TST('N', -1);
  TST('B', 2); TST('O', -2);
  TST('C', 3); TST('P', -3);
  TST('D', 4); TST('Q', -4);
  TST('E', 5); TST('R', -5);
  TST('F', 6); TST('S', -6);
  TST('G', 7); TST('T', -7);
  TST('H', 8); TST('U', -8);
  TST('I', 9); TST('V', -9);
  TST('K', 10); TST('W', -10);
  TST('L', 11); TST('X', -11);
  TST('M', 12); TST('Y', -12);

#undef TST

  const std::size_t size = 32;
  char buffer[size] = {0};
  char * begin = buffer;
  char * end = buffer;

  GIVEN("A buffer")
    {
      WHEN("universal")
	{
	  WHEN("Static")
	    {
	      REQUIRE(((std::size_t)universal_timezone<config::static_, align::right, ' ', action::size>(nullptr, universal_timezone::GMT) == 3));
	    }
	}

      WHEN("north american")
	{
	  WHEN("Static")
	    {
	      REQUIRE(((std::size_t)north_american_timezone<config::static_, action::size>(nullptr, north_american_timezone::CST) == 3));
	    }
	}

      WHEN("military")
	{
	  WHEN("Static")
	    {
	      REQUIRE(((std::size_t)military_timezone<config::static_, action::size>(nullptr, 4) == 1));

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = military_timezone<config::static_, action::prepare>(begin, 10);

		      REQUIRE(end - begin == 1);
		      REQUIRE(std::string(begin, end) == "K");
		    }
		}
	    }
	}

      WHEN("differential")
	{
	  WHEN("Static")
	    {
	      REQUIRE(((std::size_t)differential_timezone<config::static_, action::size>(nullptr, true, 12, 54) == 5));

	      GIVEN("Size")
		{
		  THEN("Prepare")
		    {
		      end = differential_timezone<config::static_, action::prepare>(begin, true, 14, 45);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "+1445");
		    }
		}
	    }
	}
    }
}

SCENARIO("Date", "[date]")
{
  {
    const std::size_t size = 32;
    char buffer[size] = {0};
    char * begin = buffer;
    char * end = buffer;

    GIVEN("A buffer")
      {
	WHEN("day month year")
	  {
	    WHEN("Leading space and 4 digits year")
	      {
		REQUIRE(((std::size_t)day_month_year<config::dynamic, false, ' ', true, action::size>(nullptr, 0, 0, 0) == 11));

		GIVEN("Size")
		  {
		    THEN("Prepare")
		      {
			end = day_month_year<config::dynamic, false, ' ', true, action::prepare>(begin, 5, 4, 0);

			REQUIRE(end - begin == 11);
			REQUIRE(std::string(begin, end) == "05 Apr 0000");

			THEN("Write or reset")
			  {
			    end = day_month_year<config::dynamic, false, ' ', true, action::write>(begin, 24, 12, 1901);

			    REQUIRE(end - begin == 11);
			    REQUIRE(std::string(begin, end) == "24 Dec 1901");
			  }
		      }
		  }
	      }

	    WHEN("Leading space and 2 digits year")
	      {
		REQUIRE(((std::size_t)day_month_year<config::dynamic, false, ' ', false, action::size>(nullptr, 0, 0, 0) == 9));

		GIVEN("Size")
		  {
		    THEN("Prepare")
		      {
			end = day_month_year<config::dynamic, false, ' ', false, action::prepare>(begin, 5, 4, 0);

			REQUIRE(end - begin == 9);
			REQUIRE(std::string(begin, end) == "05 Apr 00");

			THEN("Write or reset")
			  {
			    end = day_month_year<config::dynamic, false, ' ', false, action::write>(begin, 24, 12, 1901);

			    REQUIRE(end - begin == 9);
			    REQUIRE(std::string(begin, end) == "24 Dec 01");
			  }
		      }
		  }
	      }
	  }

	WHEN("month day")
	  {
	    REQUIRE(((std::size_t)month_day<config::dynamic, action::size>(nullptr, 0, 0) == 6));

	    GIVEN("Size")
	      {
		THEN("Prepare")
		  {
		    end = month_day<config::dynamic, action::prepare>(begin, 1, 3);

		    REQUIRE(end - begin == 6);
		    REQUIRE(std::string(begin, end) == "Jan 03");

		    THEN("Write or reset")
		      {
			end = month_day<config::dynamic, action::prepare>(begin, 12, 31);

			REQUIRE(end - begin == 6);
			REQUIRE(std::string(begin, end) == "Dec 31");
		      }
		  }
	      }
	  }
      }

    struct tm date_time;

    date_time.tm_wday = 4;
    date_time.tm_mday = 3;
    date_time.tm_mon = 1 - 1;
    date_time.tm_year = 2001 - 1900;
    date_time.tm_hour = 23;
    date_time.tm_min = 32;
    date_time.tm_sec = 57;

    WHEN("asc")
      {
	REQUIRE(((std::size_t)asc::date<config::static_, action::size>(nullptr, 0, 0, 0, 0, 0, 0, 0) == 24));

	GIVEN("Size")
	  {
	    char buffer[size] = {0};
	    char * begin = buffer;
	    char * end = buffer;

	    GIVEN("A buffer")
	      {
		THEN("Prepare")
		  {
		    end = asc::date<config::static_, action::prepare>(begin, date_time);

		    REQUIRE(end - begin == 24);
		    REQUIRE(std::string(begin, end) == "Thu Jan 03 23:32:57 2001");
		  }
	      }
	  }
      }

    WHEN("rfc822")
      {
	typedef universal_timezone_t<config::static_, align::left, ' '> timezone_t;

	WHEN("Static")
	  {
	    WHEN("Handle weekday and seconds")
	      {
		std::size_t size = (std::size_t)rfc822::date<config::static_, true, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

		GIVEN("Size")
		  {
		    char buffer[size] = {0};
		    char * begin = buffer;
		    char * end = buffer;

		    GIVEN("A buffer")
		      {
			THEN("Prepare, write or reset")
			  {
			    end = rfc822::date<config::static_, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32:57 GMT");

			    date_time.tm_mday = 12;

			    end = rfc822::date<config::static_, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 12 Jan 01 23:32:57 GMT");
			  }
		      }
		  }
	      }

	    WHEN("Handle weekday but not seconds")
	      {
		std::size_t size = (std::size_t)rfc822::date<config::static_, true, false, timezone_t, action::size>(nullptr, date_time, timezone_t());

		GIVEN("Size")
		  {
		    char buffer[size] = {0};
		    char * begin = buffer;
		    char * end = buffer;

		    GIVEN("A buffer")
		      {
			THEN("Prepare, write or reset")
			  {
			    end = rfc822::date<config::static_, true, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32 GMT");

			    date_time.tm_mday = 12;

			    end = rfc822::date<config::static_, true, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 12 Jan 01 23:32 GMT");
			  }
		      }
		  }
	      }

	    WHEN("Handle seconds but not weekday")
	      {
		std::size_t size = (std::size_t)rfc822::date<config::static_, false, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

		GIVEN("Size")
		  {
		    char buffer[size] = {0};
		    char * begin = buffer;
		    char * end = buffer;

		    GIVEN("A buffer")
		      {
			THEN("Prepare, write or reset")
			  {
			    end = rfc822::date<config::static_, false, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "03 Jan 01 23:32:57 GMT");

			    date_time.tm_mday = 12;

			    end = rfc822::date<config::static_, false, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "12 Jan 01 23:32:57 GMT");
			  }
		      }
		  }
	      }

	    WHEN("Do not handle weekday nor seconds")
	      {
		std::size_t size = (std::size_t)rfc822::date<config::static_, false, false, timezone_t, action::size>(nullptr, date_time, timezone_t());

		GIVEN("Size")
		  {
		    char buffer[size] = {0};
		    char * begin = buffer;
		    char * end = buffer;

		    GIVEN("A buffer")
		      {
			THEN("Prepare, write or reset")
			  {
			    end = rfc822::date<config::static_, false, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "03 Jan 01 23:32 GMT");

			    date_time.tm_mday = 12;

			    end = rfc822::date<config::static_, false, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "12 Jan 01 23:32 GMT");
			  }
		      }
		  }
	      }
	  }

	WHEN("Dynamic")
	  {
	    WHEN("Handle weekday and seconds")
	      {
		std::size_t size = (std::size_t)rfc822::date<config::dynamic, true, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

		GIVEN("Size")
		  {
		    GIVEN("A buffer")
		      {
			char buffer[size] = {0};
			char * begin = buffer;
			char * end = buffer;

			THEN("Prepare")
			  {
			    end = rfc822::date<config::dynamic, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32:57 GMT");

			    THEN("Reset")
			      {
				date_time.tm_mday = 11;

				end = rfc822::date<config::dynamic, true, true, timezone_t, action::reset>(begin, date_time, timezone_t());

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == "Thu, 11 Jan 01 23:32:57 GMT");
			      }
			  }
		      }
		  }
	      }
	  }
      }

    WHEN("rfc850")
      {
	typedef universal_timezone_t<config::static_, align::left, ' '> timezone_t;

	WHEN("Static")
	  {
	    std::size_t size = (std::size_t)rfc850::date<config::static_, timezone_t, action::size>(nullptr, date_time, timezone_t());

	    GIVEN("Size")
	      {
		char buffer[size] = {0};
		char * begin = buffer;
		char * end = buffer;

		GIVEN("A buffer")
		  {
		    THEN("Prepare")
		      {
			end = rfc850::date<config::static_, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thursday, 03-Jan-01 23:32:57 GMT");
		      }
		  }
	      }
	  }

	WHEN("Dynamic")
	  {
	    std::size_t size = (std::size_t)rfc850::date<config::dynamic, timezone_t, action::size>(nullptr, date_time, timezone_t());

	    GIVEN("Size")
	      {
		char buffer[size] = {0};
		char * begin = buffer;
		char * end = buffer;

		GIVEN("A buffer")
		  {
		    THEN("Prepare")
		      {
			end = rfc850::date<config::dynamic, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == " Thursday, 03-Jan-01 23:32:57 GMT");

			THEN("Reset")
			  {
			    date_time.tm_wday = 0;

			    end = rfc850::date<config::dynamic, timezone_t, action::reset>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "   Sunday, 03-Jan-01 23:32:57 GMT");
			  }
		      }
		  }
	      }
	  }
      }

    WHEN("rfc1123")
      {
	typedef universal_timezone_t<config::static_, align::left, ' '> timezone_t;

	WHEN("Static")
	  {
	    std::size_t size = (std::size_t)rfc1123::date<config::static_, true, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

	    GIVEN("Size")
	      {
		char buffer[size] = {0};
		char * begin = buffer;
		char * end = buffer;

		GIVEN("A buffer")
		  {
		    THEN("Prepare")
		      {
			end = rfc1123::date<config::static_, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thu, 03 Jan 2001 23:32:57 GMT");
		      }
		  }
	      }
	  }
      }
  }
}

template<class Iterator>
struct handler_t
{
  template<config Config, action Action>
  char * operator () (char * buffer, const Iterator & it) const
  {
    if(Action != action::size)
      {
	std::memcpy(buffer, it->first, it->second);
      }

    return buffer + it->second;
  }
};

struct separator_t
{
  template<config Config, action Action>
  char * operator () (char * buffer) const
  {
    buffer = space<Config, Action>(buffer);

    return buffer;
  }
};

SCENARIO("Container", "[container]")
{
  typedef std::vector<std::pair<const char *, std::size_t> > list_type;

  list_type list;
  list.push_back(std::make_pair("Hello", 5));
  list.push_back(std::make_pair("world", 5));
  list.push_back(std::make_pair("!", 1));

  const char * data = "Hello world !";

  typename list_type::const_iterator cbegin = list.cbegin();
  typename list_type::const_iterator cend = list.cend();

  typename list_type::const_reverse_iterator crbegin = list.crbegin();
  typename list_type::const_reverse_iterator crend = list.crend();

  GIVEN("A container and its iterators")
    {
      handler_t<typename list_type::const_iterator > handler;
      handler_t<typename list_type::const_reverse_iterator > reverse_handler;

      separator_t separator;

      GIVEN("Handlers and separator functors")
	{
	  WHEN("Container process")
	    {
	      char buffer[1024] = {0};
	      char * begin = buffer;
	      char * end = buffer + 1024;

	      GIVEN("A buffer and a handler with a separator")
		{
		  WHEN("Static")
		    {
		      WHEN("Left-aligned")
			{
			  std::size_t size = (std::size_t)container_process<config::static_, align::left, action::size>(nullptr, cbegin, cend, handler, separator);

			  REQUIRE(size == std::strlen(data));

			  GIVEN("Size")
			    {
			      THEN("Prepare, write or reset")
				{
				  end = container_process<config::static_, align::left, action::prepare>(begin, cbegin, cend, handler, separator);

				  REQUIRE(end - begin == std::strlen(data));
				  REQUIRE(std::string(begin, end) == data);
				}
			    }
			}

		      WHEN("Right-aligned")
			{
			  std::size_t size = -(std::size_t)container_process<config::static_, align::right, action::size>(nullptr, crbegin, crend, reverse_handler, separator);

			  REQUIRE(size == std::strlen(data));

			  GIVEN("Size")
			    {
			      THEN("Prepare, write or reset")
				{
				  begin = container_process<config::static_, align::right, action::prepare>(begin + size, crbegin, crend, reverse_handler, separator);

				  REQUIRE(std::string(begin, begin + size) == data);
				}
			    }
			}
		    }

		  WHEN("Dynamic")
		    {
		      WHEN("Left-aligned")
			{
			  REQUIRE(((std::size_t)container_process<config::dynamic, align::left, action::size>(nullptr, cbegin, cend, handler, separator) == std::strlen(data)));

			  WHEN("Prepare, write or reset")
			    {
			      end = container_process<config::dynamic, align::left, action::write>(begin, cbegin, cend, handler, separator);

			      REQUIRE(end - begin == std::strlen(data));
			      REQUIRE(std::string(begin, end) == data);
			    }
			}

		      WHEN("Right-aligned")
			{
			  REQUIRE(((std::size_t)container_process<config::dynamic, align::right, action::size>(nullptr, crbegin, crend, reverse_handler, separator) == -std::strlen(data)));

			  GIVEN("Size")
			    {
			      THEN("Prepare, write or reset")
				{
				  begin = container_process<config::dynamic, align::right, action::write>(begin + std::strlen(data), crbegin, crend, reverse_handler, separator);

				  REQUIRE(std::string(begin, begin + std::strlen(data)) == data);
				}
			    }
			}
		    }
		}
	    }

	  WHEN("Container")
	    {
	      std::size_t max_length = 0;

	      WHEN("Static")
		{
		  WHEN("Right-aligned")
		    {
		      std::size_t size = (std::size_t)container<config::static_, align::right, ' ', action::size>(nullptr, crbegin, crend, max_length, reverse_handler, separator);

		      char buffer[size] = {0};
		      char * begin = buffer;
		      char * end = buffer + size;

		      WHEN("Prepare")
			{
			  end = container<config::static_, align::right, ' ', action::prepare>(begin, crbegin, crend, max_length, reverse_handler, separator);

			  REQUIRE(end - begin == size);
			  REQUIRE(std::string(begin, end) == data);

			  THEN("Write")
			    {
			      end = container<config::static_, align::right, ' ', action::write>(begin, crbegin, crend, max_length, reverse_handler, separator);

			      REQUIRE(end - begin == size);
			      REQUIRE(std::string(begin, end) == data);
			    }
			}
		    }

		  WHEN("Left-aligned")
		    {

		    }
		}

	      WHEN("Dynamic")
		{
		  WHEN("Right-aligned")
		    {
		      std::size_t max_length = 64;

		      std::size_t size = (std::size_t)container<config::dynamic, align::right, ' ', action::size>(nullptr, crbegin, crend, max_length, reverse_handler, separator);

		      REQUIRE(size == max_length);

		      char buffer[size] = {0};
		      char * begin = buffer;
		      char * end = buffer + size;

		      WHEN("Prepare")
			{
			  end = container<config::dynamic, align::right, ' ', action::prepare>(begin, crbegin, crend, max_length, reverse_handler, separator);

			  REQUIRE(end - begin == size);
			  REQUIRE(size == max_length);
			  REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

			  THEN("Write")
			    {
			      end = container<config::dynamic, align::right, ' ', action::write>(begin, crbegin, crend, max_length, reverse_handler, separator);

			      REQUIRE(end - begin == size);
			      REQUIRE(size == max_length);
			      REQUIRE(std::string(begin + size - std::strlen(data), end) == data);

			      THEN("Reset")
				{
				  end = container<config::dynamic, align::right, ' ', action::reset>(begin, crbegin, crend, max_length, reverse_handler, separator);

				  REQUIRE(end - begin == size);
				  REQUIRE(size == max_length);
				  REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

				  THEN("Rewrite")
				    {
				      list.pop_back();
				      list.pop_back();

				      list.push_back(std::make_pair("world,", 6));
				      list.push_back(std::make_pair("once again!", 11));

				      const char * data2 = "Hello world, once again!";

				      crbegin = list.crbegin();
				      crend = list.crend();

				      end = container<config::dynamic, align::right, ' ', action::write>(begin, crbegin, crend, max_length, reverse_handler, separator);

				      REQUIRE(end -  begin == size);
				      REQUIRE(size == max_length);
				      REQUIRE(std::string(begin, end) == std::string(max_length - std::strlen(data2), ' ') + data2);
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
}
