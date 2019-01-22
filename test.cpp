#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <buffer_handle/boolean.hpp>
#include <buffer_handle/character.hpp>
#include <buffer_handle/container.hpp>
#include <buffer_handle/date.hpp>
#include <buffer_handle/nothing.hpp>
#include <buffer_handle/number.hpp>
#include <buffer_handle/bitset.hpp>
#include <buffer_handle/string.hpp>
#include <buffer_handle/time.hpp>
#include <buffer_handle/timezone.hpp>
#include <buffer_handle/token.hpp>

#include <buffer_handle/helper.hpp>

#include <buffer_handle/adapter/itoa/to_string.hpp>

#include <buffer_handle/test.hpp>

using namespace buffer_handle;

SCENARIO("Boolean", "[boolean]")
{
  const char pad = ' ';

  WHEN("Static, any case, left or right aligned, any pad")
    {
      REQUIRE(((std::size_t)boolean<config::static_, case_::lower, align::left, pad, action::size>(nullptr, true) == 4));

      GIVEN("Size")
	{
	  GIVEN_A_BUFFER(4)
	    {
	      THEN("Prepare true")
		{
		  end = boolean<config::static_, case_::lower, align::left, pad, action::prepare>(begin, true);

		  REQUIRE(end - begin == 4);
		  REQUIRE(std::string(begin, end) == "true");
		}

	      THEN("Prepare false")
		{
		  end = boolean<config::static_, case_::lower, align::left, pad, action::prepare>(begin, false);

		  REQUIRE(end - begin == 5);
		  REQUIRE(std::string(begin, end) == "false");
		}
	    }
	}
    }

  WHEN("Dynamic")
    {
      WHEN("Left-aligned")
	{
	  REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::left, pad, action::size>(nullptr, true) == 5));

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(5)
		{
		  THEN("Prepare")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::prepare>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "true ");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::prepare>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }

		  THEN("Write")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::write>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "true ");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::write>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }

		  THEN("Reset")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::reset>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "true ");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::left, pad, action::reset>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }
		}
	    }
	}

      WHEN("Right-aligned")
	{
	  REQUIRE(((std::size_t)boolean<config::dynamic, case_::lower, align::right, pad, action::size>(nullptr, true) == 5));

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(5)
		{
		  THEN("Prepare")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::prepare>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == " true");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::prepare>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }

		  THEN("Write")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::write>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == " true");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::write>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }

		  THEN("Reset")
		    {
		      WHEN("True")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::reset>(begin, true);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == " true");
			}

		      WHEN("False")
			{
			  end = boolean<config::dynamic, case_::lower, align::right, pad, action::reset>(begin, false);

			  REQUIRE(end - begin == 5);
			  REQUIRE(std::string(begin, end) == "false");
			}
		    }
		}
	    }
	}
    }

  WHEN("Single digit boolean")
    {
      char c;
      const char t = 'a';
      const char f = 'b';

      WHEN("Static true")
	{
	  char * end = boolean<config::static_, action::prepare, f, t>(&c, true);

	  REQUIRE(end == &c + 1);
	  REQUIRE(c == t);
	}

      WHEN("Static false")
	{
	  char * end = boolean<config::static_, action::prepare, f, t>(&c, false);

	  REQUIRE(end == &c + 1);
	  REQUIRE(c == f);
	}
    }
}

SCENARIO("Character", "[character]")
{
  char c = '\0';
  char * begin = &c;
  char * end = &c;

  WHEN("Static")
    {
      REQUIRE((std::size_t)character<config::static_, action::size>(nullptr, 0) == 1);

      end = character<config::static_, action::prepare>(begin, 'c');

      REQUIRE(end - begin == 1);
      REQUIRE(c == 'c');
    }

  WHEN("Dynamic")
    {
      REQUIRE((std::size_t)character<config::static_, action::size>(nullptr, 0) == 1);

      WHEN("Prepare")
	{
	  end = character<config::dynamic, action::prepare>(begin, 'c');

	  REQUIRE(end - begin == 1);
	  REQUIRE(c == 'c');
	}

      WHEN("Write")
	{
	  end = character<config::dynamic, action::write>(begin, 'c');

	  REQUIRE(end - begin == 1);
	  REQUIRE(c == 'c');
	}

      WHEN("Reset")
	{
	  end = character<config::dynamic, action::reset>(begin, 'c');

	  REQUIRE(end - begin == 1);
	  REQUIRE(c == 'c');
	}
    }
}

struct element_handler_t
{
  template<config Config, action Action>
  char * handle(char * buffer, std::pair<const char *, std::size_t> element) const
  {
    return string<Config, Action>(buffer, element.first, element.second);
  }
};

template<std::size_t Size>
struct element_handler_with_max_size_t
{
  template<config Config, action Action>
  char * handle(char * buffer, std::pair<const char *, std::size_t> element) const
  {
    assert(element.second <= Size);

    switch(Action)
      {
      case action::size: return buffer + Size;
      default: std::memcpy(buffer, element.first, element.second);
      }

    return buffer + element.second;
  }
};

using separator_t = character_separator_t<' '>;

SCENARIO("Container", "[container]")
{
  typedef std::vector<std::pair<const char *, std::size_t> > list_type;

  list_type list;
  list.push_back(std::make_pair("Hello", 5));
  list.push_back(std::make_pair("world", 5));
  list.push_back(std::make_pair("!", 1));

  const char * data = "Hello world !";
  const std::size_t length = std::strlen(data);

  typename list_type::const_iterator cbegin = list.cbegin();
  typename list_type::const_iterator cend = list.cend();

  typename list_type::const_reverse_iterator crbegin = list.crbegin();
  typename list_type::const_reverse_iterator crend = list.crend();

  GIVEN("A container and its iterators")
    {
      element_handler_t element_handler;

      separator_t separator;

      GIVEN("Handler and separator functors")
	{
	  WHEN("Container process")
	    {
	      char buffer[64] = {0};
	      char * begin = buffer;
	      char * end = buffer + 64;

	      GIVEN("A buffer and a handler with a separator")
		{
		  WHEN("Static")
		    {
		      WHEN("Left-aligned")
			{
			  std::size_t size = (std::size_t)details::container_process<config::static_, align::left, action::size>(nullptr, cbegin, cend, element_handler, separator);

			  REQUIRE(size == std::strlen(data));

			  GIVEN("Size")
			    {
			      THEN("Prepare")
				{
				  end = details::container_process<config::static_, align::left, action::prepare>(begin, cbegin, cend, element_handler, separator);

				  REQUIRE(end - begin == std::strlen(data));
				  REQUIRE(std::string(begin, end) == data);
				}
			    }
			}

		      WHEN("Right-aligned")
			{
			  std::size_t size = -(std::size_t)details::container_process<config::static_, align::right, action::size>(nullptr, crbegin, crend, element_handler, separator);

			  REQUIRE(size == std::strlen(data));

			  GIVEN("Size")
			    {
			      THEN("Prepare")
				{
				  begin = details::container_process<config::static_, align::right, action::prepare>(begin + size, crbegin, crend, element_handler, separator);

				  REQUIRE(std::string(begin, begin + size) == data);
				}
			    }
			}
		    }

		  WHEN("Dynamic")
		    {
		      WHEN("Left-aligned")
			{
			  std::size_t size = (std::size_t)details::container_process<config::dynamic, align::left, action::size>(nullptr, cbegin, cend, element_handler, separator);

			  REQUIRE(size == std::strlen(data));

			  THEN("Prepare, write or reset")
			    {
			      end = details::container_process<config::dynamic, align::left, action::write>(begin, cbegin, cend, element_handler, separator);

			      REQUIRE(end - begin == std::strlen(data));
			      REQUIRE(std::string(begin, end) == data);
			    }
			}

		      WHEN("Right-aligned")
			{
			  std::size_t size = (std::size_t)details::container_process<config::dynamic, align::right, action::size>(nullptr, crbegin, crend, element_handler, separator);

			  REQUIRE(size == -std::strlen(data));

			  GIVEN("Size")
			    {
			      THEN("Prepare, write or reset")
				{
				  begin = details::container_process<config::dynamic, align::right, action::write>(begin + std::strlen(data), crbegin, crend, element_handler, separator);

				  REQUIRE(std::string(begin, begin + std::strlen(data)) == data);
				}
			    }
			}
		    }
		}
	    }

	  WHEN("Container")
	    {
	      const char pad = ' ';

	      WHEN("Static")
		{
		  const std::size_t max_length = 0;

		  WHEN("Left-aligned")
		    {
		      std::size_t size = (std::size_t)container<config::static_, align::left, pad, action::size>(nullptr, cbegin, cend, max_length, element_handler, separator);

		      GIVEN("Size")
			{
			  GIVEN_A_BUFFER(size)
			  {
			    THEN("Prepare")
			      {
				end = container<config::static_, align::left, pad, action::prepare>(begin, cbegin, cend, max_length, element_handler, separator);

				REQUIRE(end - begin == length);
				REQUIRE(std::string(begin, end) == data);
			      }
			  }
			}
		    }

		  WHEN("Right-aligned")
		    {
		      std::size_t size = (std::size_t)container<config::static_, align::right, pad, action::size>(nullptr, crbegin, crend, max_length, element_handler, separator);

		      GIVEN("Size")
			{
			  GIVEN_A_BUFFER(size)
			  {
			    THEN("Prepare")
			      {
				end = container<config::static_, align::right, pad, action::prepare>(begin, crbegin, crend, max_length, element_handler, separator);

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == data);
			      }
			  }
			}
		    }
		}

	      WHEN("Dynamic")
		{
		  const std::size_t max_length = 64;

		  WHEN("Left-aligned")
		    {
		      const std::size_t size = (std::size_t)container<config::dynamic, align::left, pad, action::size>(nullptr, cbegin, cend, max_length, element_handler, separator);

		      REQUIRE(size == max_length);

		      GIVEN("Size")
			{
			  GIVEN_A_BUFFER(size)
			  {
			    THEN("Prepare")
			      {
				end = container<config::dynamic, align::left, pad, action::prepare>(begin, cbegin, cend, max_length, element_handler, separator);

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

				THEN("Write")
				  {
				    list.pop_back();
				    list.pop_back();

				    list.push_back(std::make_pair("world,", 6));
				    list.push_back(std::make_pair("once again!", 11));

				    const char * data2 = "Hello world, once again!";

				    crbegin = list.crbegin();
				    crend = list.crend();

				    end = container<config::dynamic, align::left, pad, action::write>(begin, cbegin, cend, max_length, element_handler, separator);

				    REQUIRE(end -  begin == size);
				    REQUIRE(std::string(begin, end) == data2 + std::string(max_length - std::strlen(data2), pad));

				    THEN("Reset")
				      {
					end = container<config::dynamic, align::left, pad, action::reset>(begin, cend, cend, max_length, element_handler, separator);

					REQUIRE(end - begin == size);
					REQUIRE(std::string(begin, end) == std::string(max_length, pad));
				      }
				  }
			      }
			  }
			}
		    }

		  WHEN("Right-aligned")
		    {
		      const std::size_t size = (std::size_t)container<config::dynamic, align::right, pad, action::size>(nullptr, crbegin, crend, max_length, element_handler, separator);

		      REQUIRE(size == max_length);

		      GIVEN("Size")
			{
			  GIVEN_A_BUFFER(size)
			  {
			    THEN("Prepare")
			      {
				end = container<config::dynamic, align::right, pad, action::prepare>(begin, crbegin, crend, max_length, element_handler, separator);

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

				THEN("Write")
				  {
				    list.pop_back();
				    list.pop_back();

				    list.push_back(std::make_pair("world,", 6));
				    list.push_back(std::make_pair("once again!", 11));

				    const char * data2 = "Hello world, once again!";

				    crbegin = list.crbegin();
				    crend = list.crend();

				    end = container<config::dynamic, align::right, pad, action::write>(begin, crbegin, crend, max_length, element_handler, separator);

				    REQUIRE(end -  begin == size);
				    REQUIRE(std::string(begin, end) == std::string(max_length - std::strlen(data2), pad) + data2);

				    THEN("Reset")
				      {
					end = container<config::dynamic, align::right, pad, action::reset>(begin, crend, crend, max_length, element_handler, separator);

					REQUIRE(end - begin == size);
					REQUIRE(std::string(begin, end) == std::string(max_length, pad));
				      }
				  }
			      }
			  }
			}
		    }
		}
	    }

	  WHEN("Dynamic container")
	    {
	      typename list_type::const_iterator current = cbegin;

	      element_handler_with_max_size_t<6> element_handler;

	      WHEN("Every thing can fit")
		{
		  std::size_t max_length = 32;

		  GIVEN_A_BUFFER(max_length)
		  {
		    THEN("Prepare")
		      {
			end = container<align::left, ' ', action::prepare>(begin, cbegin, current, cend, max_length, element_handler, separator);

			REQUIRE(end - begin == max_length);
			REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

			THEN("Write")
			  {
			    end = container<align::left, ' ', action::write>(begin, cbegin, current, cend, max_length, element_handler, separator);

			    REQUIRE(end - begin == length);
			    REQUIRE(std::string(begin, end) == data);
			  }
		      }
		  }
		}

	      WHEN("Some can fit")
		{
		  std::size_t max_length = 8;

		  GIVEN_A_BUFFER(max_length)
		  {
		    THEN("Prepare")
		      {
			end = container<align::left, ' ', action::prepare>(begin, cbegin, current, cend, max_length, element_handler, separator);

			REQUIRE(end - begin == max_length);
			REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

			THEN("Write")
			  {
			    end = container<align::left, ' ', action::write>(begin, cbegin, current, cend, max_length, element_handler, separator);

			    REQUIRE(end - begin == 5);
			    REQUIRE(std::string(begin, end) == "Hello");

			    THEN("Write")
			      {
				end = container<align::left, ' ', action::write>(begin, cbegin, current, cend, max_length, element_handler, separator);

				REQUIRE(end - begin == 6);
				REQUIRE(std::string(begin, end) == " world");

				THEN("Write")
				  {
				    end = container<align::left, ' ', action::write>(begin, cbegin, current, cend, max_length, element_handler, separator);

				    REQUIRE(end - begin == 2);
				    REQUIRE(std::string(begin, end) == " !");
				  }
			      }
			  }
		      }
		  }
		}

	      WHEN("None can fit")
		{
		  std::size_t max_length = 2;

		  GIVEN_A_BUFFER(max_length)
		  {
		    THEN("Prepare")
		      {
			end = container<align::left, ' ', action::prepare>(begin, cbegin, current, cend, max_length, element_handler, separator);

			REQUIRE(end - begin == max_length);
			REQUIRE(std::string(begin, end) == std::string(max_length, ' '));

			THEN("Write")
			  {
			    end = container<align::left, ' ', action::write>(begin, cbegin, current, cend, max_length, element_handler, separator);

			    REQUIRE(end - begin == 0);
			  }
		      }
		  }
		}
	    }

	  WHEN("container_t<config::dynamic>")
	    {
	      typedef container_t<config::dynamic, align::left, ' '> container_type;

	      container_type container = container_type();

	      container.set_max_length(64);

	      std::size_t size = (std::size_t)container.handle<action::size, typename list_type::const_iterator, const element_handler_t, const separator_t>(nullptr, cbegin, cend, element_handler_t(), separator_t());

	      REQUIRE(size == 64);
	    }

	  WHEN("container_t<config::dynamic, ..., true>")
	    {
	      typedef container_t<config::dynamic, align::left, ' ', true> container_type;

	      container_type container = container_type();

	      container.set_max_length(128);

	      std::size_t size = (std::size_t)container.handle<action::size, typename list_type::const_iterator, const element_handler_t, const separator_t>(nullptr, cbegin, cend, element_handler_t(), separator_t());

	      REQUIRE(size == 128);
	    }
	}
    }
}

SCENARIO("Date", "[date]")
{
  WHEN("day month year")
    {
      WHEN("Static or dynamic")
	{
	  WHEN("Leading space and 4 digits year")
	    {
	      std::size_t size = (std::size_t)day_month_year<config::dynamic, '\0', ' ', true, action::size>(nullptr, 0, 0, 0);

	      REQUIRE(size == 11);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = day_month_year<config::dynamic, '\0', ' ', true, action::prepare>(begin, 5, 4, 0);

			REQUIRE(end - begin == 11);
			REQUIRE(std::string(begin, end) == "05 Apr 0000");

			THEN("Write")
			  {
			    end = day_month_year<config::dynamic, '\0', ' ', true, action::write>(begin, 24, 12, 1901);

			    REQUIRE(end - begin == 11);
			    REQUIRE(std::string(begin, end) == "24 Dec 1901");
			  }
		      }
		  }
		}
	    }

	  WHEN("Leading space and 2 digits year")
	    {
	      std::size_t size = (std::size_t)day_month_year<config::dynamic, ' ', ' ', false, action::size>(nullptr, 0, 0, 0);

	      REQUIRE(size == 9);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = day_month_year<config::dynamic, ' ', ' ', false, action::prepare>(begin, 5, 4, 0);

			REQUIRE(end - begin == 9);
			REQUIRE(std::string(begin, end) == " 5 Apr 00");

			THEN("Write or reset")
			  {
			    end = day_month_year<config::dynamic, ' ', ' ', false, action::write>(begin, 24, 12, 1901);

			    REQUIRE(end - begin == 9);
			    REQUIRE(std::string(begin, end) == "24 Dec 01");
			  }
		      }
		  }
		}
	    }
	}
    }

  WHEN("month day")
    {
      WHEN("Static or dynamic")
	{
	  std::size_t size = (std::size_t)month_day<config::dynamic, action::size>(nullptr, 0, 0);

	  REQUIRE(size == 6);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(size)
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
    }

  std::tm date_time;

  date_time.tm_wday = 4;
  date_time.tm_mday = 3;
  date_time.tm_mon = 1 - 1;
  date_time.tm_year = 2001 - 1900;
  date_time.tm_hour = 23;
  date_time.tm_min = 32;
  date_time.tm_sec = 57;

  WHEN("asc")
    {
      std::size_t size = (std::size_t)asc::date<config::static_, action::size>(nullptr, std::tm());

      REQUIRE(size == 24);

      GIVEN("Size")
	{
	  GIVEN_A_BUFFER(size)
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
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare, write or reset")
		      {
			end = rfc822::date<config::static_, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32:57 GMT");
		      }

		    THEN("Prepare, write or reset")
		      {
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
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare, write or reset")
		      {
			end = rfc822::date<config::static_, true, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32 GMT");
		      }

		    THEN("Prepare, write or reset")
		      {
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
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare, write or reset")
		      {
			end = rfc822::date<config::static_, false, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "03 Jan 01 23:32:57 GMT");
		      }

		    THEN("Prepare, write or reset")
		      {
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
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare, write or reset")
		      {
			end = rfc822::date<config::static_, false, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "03 Jan 01 23:32 GMT");
		      }

		    THEN("Prepare, write or reset")
		      {
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
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = rfc822::date<config::dynamic, true, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32:57 GMT");

			THEN("Write")
			  {
			    date_time.tm_hour = 2;

			    end = rfc822::date<config::dynamic, true, true, timezone_t, action::write>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 02:32:57 GMT");

			    THEN("Reset")
			      {
				date_time.tm_mday = 11;

				end = rfc822::date<config::dynamic, true, true, timezone_t, action::reset>(begin, date_time, timezone_t());

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == "Thu, 11 Jan 01 02:32:57 GMT");
			      }
			  }
		      }
		  }
		}
	    }

	  WHEN("Handle weekday but not seconds")
	    {
	      std::size_t size = (std::size_t)rfc822::date<config::dynamic, true, false, timezone_t, action::size>(nullptr, date_time, timezone_t());

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = rfc822::date<config::dynamic, true, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 23:32 GMT");

			THEN("Write")
			  {
			    date_time.tm_hour = 2;

			    end = rfc822::date<config::dynamic, true, false, timezone_t, action::write>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "Thu, 03 Jan 01 02:32 GMT");

			    THEN("Reset")
			      {
				date_time.tm_mday = 11;

				end = rfc822::date<config::dynamic, true, false, timezone_t, action::reset>(begin, date_time, timezone_t());

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == "Thu, 11 Jan 01 02:32 GMT");
			      }
			  }
		      }
		  }
		}
	    }

	  WHEN("Handle seconds but not weekdays")
	    {
	      std::size_t size = (std::size_t)rfc822::date<config::dynamic, false, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = rfc822::date<config::dynamic, false, true, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "03 Jan 01 23:32:57 GMT");

			THEN("Write")
			  {
			    date_time.tm_hour = 2;

			    end = rfc822::date<config::dynamic, false, true, timezone_t, action::write>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "03 Jan 01 02:32:57 GMT");

			    THEN("Reset")
			      {
				date_time.tm_mday = 11;

				end = rfc822::date<config::dynamic, false, true, timezone_t, action::reset>(begin, date_time, timezone_t());

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == "11 Jan 01 02:32:57 GMT");
			      }
			  }
		      }
		  }
		}
	    }

	  WHEN("Do not handle seconds nor weekdays")
	    {
	      std::size_t size = (std::size_t)rfc822::date<config::dynamic, false, false, timezone_t, action::size>(nullptr, date_time, timezone_t());

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = rfc822::date<config::dynamic, false, false, timezone_t, action::prepare>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "03 Jan 01 23:32 GMT");

			THEN("Write")
			  {
			    date_time.tm_hour = 2;

			    end = rfc822::date<config::dynamic, false, false, timezone_t, action::write>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "03 Jan 01 02:32 GMT");

			    THEN("Reset")
			      {
				date_time.tm_mday = 11;

				end = rfc822::date<config::dynamic, false, false, timezone_t, action::reset>(begin, date_time, timezone_t());

				REQUIRE(end - begin == size);
				REQUIRE(std::string(begin, end) == "11 Jan 01 02:32 GMT");
			      }
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
	      GIVEN_A_BUFFER(size)
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
	      GIVEN_A_BUFFER(size)
	      {
		THEN("Prepare")
		  {
		    end = rfc850::date<config::dynamic, timezone_t, action::prepare>(begin, date_time, timezone_t());

		    REQUIRE(end - begin == size);
		    REQUIRE(std::string(begin, end) == "         , 03-Jan-01 23:32:57 GMT");

		    THEN("Write")
		      {
			date_time.tm_wday = 0;

			end = rfc850::date<config::dynamic, timezone_t, action::reset>(begin, date_time, timezone_t());

			REQUIRE(end - begin == size);
			REQUIRE(std::string(begin, end) == "   Sunday, 03-Jan-01 23:32:57 GMT");

			THEN("Reset")
			  {
			    date_time = std::tm();

			    end = rfc850::date<config::dynamic, timezone_t, action::reset>(begin, date_time, timezone_t());

			    REQUIRE(end - begin == size);
			    REQUIRE(std::string(begin, end) == "   Sunday, 00-Jan-00 00:00:00 GMT");
			  }
		      }
		  }
	      }
	    }
	}
    }

  WHEN("rfc1123")//see rfc850
    {
      typedef universal_timezone_t<config::static_, align::left, ' '> timezone_t;

      WHEN("Static")
	{
	  std::size_t size = (std::size_t)rfc1123::date<config::static_, true, true, timezone_t, action::size>(nullptr, date_time, timezone_t());

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(size)
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

SCENARIO("Helper", "[helper]")
{
  static_assert(must_write(config::static_, action::prepare));
  static_assert(write_when_reset(action::reset) == action::write);

  const char pad = ' ';

  const char reference[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
  char buffer[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
  const std::size_t max_length = std::strlen(buffer);
  char * begin = buffer;
  char * end = buffer + max_length;

  GIVEN("A buffer")
    {
      WHEN("Pad left")
	{
	  std::size_t previous_length = max_length;

	  WHEN("Use previous length")
	    {
	      THEN("Keep all")
		{
		  pad_left<true, pad>(begin + max_length, end, max_length, previous_length);

		  REQUIRE(previous_length == max_length);
		  REQUIRE(std::string(begin, end) == reference);
		}

	      THEN("Keep all characters but the first 10")
		{
		  pad_left<true, pad>(begin, end - 10, max_length, previous_length);

		  REQUIRE(previous_length == 10);
		  REQUIRE(std::string(begin, end) == (std::string(max_length - 10, pad) + std::string(reference).substr(max_length - 10)));
		}
	    }

	  WHEN("Not use previous length")
	    {
	      THEN("Keep all characters but the first 10")
		{
		  pad_left<false, pad>(begin, end - 10, max_length, previous_length);

		  REQUIRE(std::string(begin, end) == std::string(max_length - 10, pad) + std::string(reference).substr(max_length - 10));
		}
	    }
	}

      WHEN("Pad right")
	{
	  std::size_t previous_length = max_length;

	  WHEN("Use previous length")
	    {
	      THEN("Keep all")
		{
		  pad_right<true, pad>(begin, end, max_length, previous_length);

		  REQUIRE(previous_length == max_length);
		  REQUIRE(std::string(begin, end) == reference);
		}

	      THEN("Keep first 10 characters")
		{
		  pad_right<true, pad>(begin, begin + 10, max_length, previous_length);

		  REQUIRE(previous_length == 10);
		  REQUIRE(std::string(begin, end) == std::string(reference).substr(0, 10) + std::string(max_length - 10, pad));
		}
	    }

	  WHEN("Not use previous length")
	    {
	      THEN("Keep first 10 characters")
		{
		  pad_right<false, pad>(begin, begin + 10, max_length, previous_length);

		  REQUIRE(std::string(begin, end) == std::string(reference).substr(0, 10) + std::string(max_length - 10, pad));
		}
	    }
	}
    }
}

SCENARIO("Itoa adapter", "[itoa, adapter]")
{
  char buffer[64] = {0};
  char * begin = buffer;
  char * end = buffer;

  WHEN("To string")
    {
      adapter::itoa::to_string_t to_string;

      WHEN("Forward")
	{
#define TEST(I, L)					\
	  THEN(#I)					\
	    {						\
	      char * end = to_string.fwd(begin, I);	\
							\
	      REQUIRE(end - begin == L);		\
	      REQUIRE(std::string(begin, end) == #I);	\
	    }

	  TEST(0, 1);
	  TEST(1, 1);
	  TEST(10, 2);
	  TEST(99, 2);
	  TEST(3993, 4);

#undef TEST
	}

      WHEN("Backward")
	{
#define TEST(I, L)					\
	  THEN(#I)					\
	    {						\
	      char * begin = to_string.bwd(end, I);	\
							\
	      REQUIRE(end - begin == L);		\
	      REQUIRE(std::string(begin, end) == #I);	\
	    }

	  TEST(0, 1);
	  TEST(1, 1);
	  TEST(10, 2);
	  TEST(99, 2);
	  TEST(3993, 4);

#undef TEST
	}
    }
}

SCENARIO("Nothing", "[nothing]")
{
  char c;

  REQUIRE(((std::size_t)nothing_t().handle<action::size>(nullptr) == 0));
  REQUIRE(nothing_t().handle<action::prepare>(&c) == &c);
  REQUIRE(nothing_t().handle<action::write>(&c) == &c);
  REQUIRE(nothing_t().handle<action::reset>(&c) == &c);
}

SCENARIO("Number", "[Number]")
{
  {
    REQUIRE(details::digits<uint8_t>(0) == 1);
    REQUIRE(details::digits<uint8_t>(1) == 1);
    REQUIRE(details::digits<uint8_t>(4) == 1);
    REQUIRE(details::digits<uint8_t>(67) == 2);
    REQUIRE(details::digits<uint8_t>(234) == 3);

    REQUIRE(details::digits<int>(-1) == 2);
    REQUIRE(details::digits<int>(-4) == 2);
    REQUIRE(details::digits<int>(-34) == 3);
  }

  const char pad = ' ';

  WHEN("Two digits number")
    {
      WHEN("Static or dynamic")
	{
	  WHEN("Leading zero")
	    {
	      REQUIRE(((std::size_t)two_digits_number<config::static_, '\0', action::size, uint8_t>(nullptr, 0) == 2));

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(2)
		    {
		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, '\0', action::prepare, uint8_t>(begin, 43);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == "43");
			}

		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, '\0', action::prepare, uint8_t>(begin, 7);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == "07");
			}

		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, '\0', action::prepare, uint8_t>(begin, 0);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == "00");
			}
		    }
		}
	    }

	  WHEN("Leading space")
	    {
	      REQUIRE(((std::size_t)two_digits_number<config::static_, pad, action::size, uint8_t>(nullptr, 0) == 2));

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(2)
		    {
		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, pad, action::prepare, uint8_t>(begin, 99);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == "99");
			}

		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, pad, action::prepare, uint8_t>(begin, 7);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == " 7");
			}

		      WHEN("Prepare")
			{
			  end = two_digits_number<config::static_, pad, action::prepare, uint8_t>(begin, 0);

			  REQUIRE(end - begin == 2);
			  REQUIRE(std::string(begin, end) == " 0");
			}
		    }
		}
	    }
	}
    }

  WHEN("Four digits number")
    {
      WHEN("Static of dynamic")
	{
	  REQUIRE(((std::size_t)four_digits_number<config::static_, action::size, uint16_t>(nullptr, 0) == 4));

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(4)
		{
		  WHEN("Prepare")
		    {
		      end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 0);

		      REQUIRE(end - begin == 4);
		      REQUIRE(std::string(begin, end) == "0000");
		    }

		  WHEN("Prepare")
		    {
		      end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 2);

		      REQUIRE(end - begin == 4);
		      REQUIRE(std::string(begin, end) == "0002");
		    }

		  WHEN("Prepare")
		    {
		      end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 56);

		      REQUIRE(end - begin == 4);
		      REQUIRE(std::string(begin, end) == "0056");
		    }

		  WHEN("Prepare")
		    {
		      end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 723);

		      REQUIRE(end - begin == 4);
		      REQUIRE(std::string(begin, end) == "0723");
		    }

		  WHEN("Prepare")
		    {
		      end = four_digits_number<config::static_, action::prepare, uint16_t>(begin, 2968);

		      REQUIRE(end - begin == 4);
		      REQUIRE(std::string(begin, end) == "2968");
		    }
		}
	    }
	}
    }

  WHEN("Integral number")
    {
      uint8_t max_digits = 0;
      const char pad = ' ';

      adapter::itoa::to_string_t itoa;

      WHEN("Static")
	{
	  WHEN("Left or right aligned, any padding")
	    {
	      std::size_t size = (std::size_t)integral_number<config::static_, align::left, pad, action::size>(nullptr, 1998, max_digits, itoa);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = integral_number<config::static_, align::left, pad, action::prepare>(begin, 1998, max_digits, itoa);

			REQUIRE(end == begin + 4);
			REQUIRE(max_digits == 4);
			REQUIRE(std::string(begin, end) == "1998");
		      }
		  }
		}
	    }
	}

      WHEN("Dynamic")
	{
	  uint8_t previous_digits = 0;
	  const uint8_t max_digits_ = 8;

	  WHEN("Left-aligned")
	    {
	      std::size_t size = (std::size_t)integral_number<config::dynamic, align::left, pad, action::size>(nullptr, 19237840, max_digits, previous_digits, itoa);

	      REQUIRE(size == max_digits_);
	      REQUIRE(previous_digits == 0);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = integral_number<config::dynamic, align::left, pad, action::prepare>(begin, 19237840, max_digits, previous_digits, itoa);

			REQUIRE(end - begin == max_digits_);
			REQUIRE(max_digits == max_digits_);
			REQUIRE(previous_digits == 0);
			REQUIRE(std::string(begin, end) == "        ");

			THEN("Write")
			  {
			    end = integral_number<config::dynamic, align::left, pad, action::write>(begin, 1923784, max_digits, previous_digits, itoa);

			    REQUIRE(end - begin == max_digits_);
			    REQUIRE(max_digits == max_digits_);
			    REQUIRE(previous_digits == 7);
			    REQUIRE(std::string(begin, end) == "1923784 ");

			    THEN("Reset")
			      {
				end = integral_number<config::dynamic, align::left, pad, action::reset>(begin, 19, max_digits, previous_digits, itoa);

				REQUIRE(end - begin == max_digits_);
				REQUIRE(max_digits == max_digits_);
				REQUIRE(previous_digits == 0);
				REQUIRE(std::string(begin, end) == std::string(max_digits_, pad));

				THEN("Write")
				  {
				    end = integral_number<config::dynamic, align::left, pad, action::write>(begin, 7326, max_digits, previous_digits, itoa);

				    REQUIRE(end - begin == max_digits_);
				    REQUIRE(max_digits == max_digits_);
				    REQUIRE(previous_digits == 4);
				    REQUIRE(std::string(begin, end) == "7326" + std::string(max_digits_ - 4, pad));
				  }
			      }
			  }
		      }
		  }
		}
	    }

	  WHEN("Right-aligned")
	    {
	      std::size_t size = (std::size_t)integral_number<config::dynamic, align::right, pad, action::size>(nullptr, 19237840, max_digits, previous_digits, itoa);

	      REQUIRE(size == max_digits_);
	      REQUIRE(previous_digits == 0);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = integral_number<config::dynamic, align::right, pad, action::prepare>(begin, 19237840, max_digits, previous_digits, itoa);

			REQUIRE(end - begin == max_digits_);
			REQUIRE(max_digits == max_digits_);
			REQUIRE(previous_digits == 0);
			REQUIRE(std::string(begin, end) == "        ");

			THEN("Write")
			  {
			    end = integral_number<config::dynamic, align::right, pad, action::write>(begin, 1923784, max_digits, previous_digits, itoa);

			    REQUIRE(end - begin == max_digits_);
			    REQUIRE(max_digits == max_digits_);
			    REQUIRE(previous_digits == 7);
			    REQUIRE(std::string(begin, end) == " 1923784");

			    THEN("Reset")
			      {
				end = integral_number<config::dynamic, align::right, pad, action::reset>(begin, 19, max_digits, previous_digits, itoa);

				REQUIRE(end - begin == max_digits_);
				REQUIRE(max_digits == max_digits_);
				REQUIRE(previous_digits == 0);
				REQUIRE(std::string(begin, end) == std::string(max_digits_, pad));

				THEN("Write")
				  {
				    end = integral_number<config::dynamic, align::right, pad, action::write>(begin, 7236, max_digits, previous_digits, itoa);

				    REQUIRE(end - begin == max_digits_);
				    REQUIRE(max_digits == max_digits_);
				    REQUIRE(previous_digits == 4);
				    REQUIRE(std::string(begin, end) == std::string(max_digits_ - 4, pad) + "7236");
				  }
			      }
			  }
		      }
		  }
		}
	    }
	}
    }

  adapter::itoa::to_string_t itoa;

  WHEN("integral_number_t<config::static_>")
    {
      typedef integral_number_t<config::static_, align::left, ' ', uint8_t> number_type;

      number_type number = number_type();

      std::size_t size = (std::size_t)number.handle<action::size>(nullptr, 11, itoa);

      REQUIRE(size == 2);
    }

  WHEN("integral_number_t<config::dynamic>")
    {
      typedef integral_number_t<config::dynamic, align::left, ' ', uint8_t> number_type;

      number_type number = number_type();

      std::size_t size = (std::size_t)number.handle<action::size>(nullptr, 183, itoa);

      REQUIRE(size == 3);
    }

  WHEN("integral_number_t<config::dynamic, ..., true>")
    {
      typedef integral_number_t<config::dynamic, align::left, ' ', uint64_t, uint8_t, true> number_type;

      number_type number = number_type();

      std::size_t size = (std::size_t)number.handle<action::size>(nullptr, 34378367, itoa);

      REQUIRE(size == 8);
    }
}

struct set_t
{
  enum class value_type {None, Alice = 1, Bob = 2, Charlie = 4, David = 8};
  static const std::size_t count = 4;
  static const char * get(value_type value)
  {
    switch(value)
      {
      case value_type::Alice: return "Alice";
      case value_type::Bob: return "Bob";
      case value_type::Charlie: return "Charlie";
      case value_type::David: return "David";
      default: return "";
      }
  }
};

set_t::value_type operator | (set_t::value_type lhs, set_t::value_type rhs)
{
  return (set_t::value_type)
    (((typename std::underlying_type<set_t::value_type>::type)lhs)
     | ((typename std::underlying_type<set_t::value_type>::type)rhs));
}

SCENARIO("Bitset", "[bitset]")
{
  character_separator_t<','> separator;

  WHEN("Static")
    {
      std::size_t size = (std::size_t)bitset<set_t, action::size>(nullptr, set_t::value_type::Alice | set_t::value_type::Bob, separator);

      GIVEN_A_BUFFER(size)
      {
	THEN("Prepare")
	  {
	    end = bitset<set_t, action::prepare>(begin, set_t::value_type::Alice | set_t::value_type::Bob, separator);

	    REQUIRE(end - begin == size);
	    REQUIRE(std::string(begin, end) == "Alice,Bob");
	  }
      }
    }

  WHEN("Dynamic")
    {
      std::size_t max_length;

      WHEN("Left-aligned")
	{
	  std::size_t size = (std::size_t)bitset<config::dynamic, align::left, ' ', set_t, action::size>(nullptr, set_t::value_type::Alice | set_t::value_type::Charlie | set_t::value_type::David, max_length, separator);

	  GIVEN_A_BUFFER(size)
	  {
	    THEN("Prepare")
	      {
		end = bitset<config::dynamic, align::left, ' ', set_t, action::prepare>(begin, set_t::value_type::Alice | set_t::value_type::Charlie | set_t::value_type::David, max_length, separator);

		REQUIRE(end - begin == size);
		REQUIRE(std::string(begin, end) == std::string(size, ' '));

		THEN("Write")
		  {
		    end = bitset<config::dynamic, align::left, ' ', set_t, action::write>(begin, set_t::value_type::Charlie, max_length, separator);

		    REQUIRE(end - begin == size);
		    REQUIRE(std::string(begin, end) == std::string("Charlie") + std::string(max_length - std::strlen("Charlie"), ' '));
		  }
	      }
	  }
	}

      WHEN("Right-aligned")
	{
	  std::size_t size = (std::size_t)bitset<config::dynamic, align::right, ' ', set_t, action::size>(nullptr, set_t::value_type::Alice | set_t::value_type::Charlie | set_t::value_type::David, max_length, separator);

	  GIVEN_A_BUFFER(size)
	  {
	    THEN("Prepare")
	      {
		end = bitset<config::dynamic, align::right, ' ', set_t, action::prepare>(begin, set_t::value_type::Alice | set_t::value_type::Charlie | set_t::value_type::David, max_length, separator);

		REQUIRE(end - begin == size);
		REQUIRE(std::string(begin, end) == std::string(size, ' '));

		THEN("Write")
		  {
		    end = bitset<config::dynamic, align::right, ' ', set_t, action::write>(begin, set_t::value_type::Charlie, max_length, separator);

		    REQUIRE(end - begin == size);
		    REQUIRE(std::string(begin, end) == std::string(max_length - std::strlen("Charlie"), ' ') + "Charlie");
		  }
	      }
	  }
	}
    }
}

SCENARIO("Time", "[time]")
{
  const char pad = ' ';

  WHEN("time_t")
    {
      //see integral_number
      uint8_t max_digits;
      adapter::itoa::to_string_t itoa;

      time_t time = 8939;

      std::size_t size = (std::size_t)time_<config::static_, align::left, pad, action::size>(nullptr, time, max_digits, itoa);

      REQUIRE(size == 4);
    }

  WHEN("Hours and minutes")
    {
      uint8_t hours = 5;
      uint8_t minutes = 23;

      WHEN("Static or dynamic")
	{
	  std::size_t size = (std::size_t)time_<config::static_, action::size>(nullptr, hours, minutes);

	  REQUIRE(size == 5);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(size)
	      {
		THEN("Prepare, write or reset")
		  {
		    end = time_<config::static_, action::prepare>(buffer, hours, minutes);

		    REQUIRE(end - begin == size);
		    REQUIRE(std::string(begin, end) == "05:23");
		  }
	      }
	    }
	}
    }

  WHEN("Hours, minutes and seconds")
    {
      uint8_t hours = 5;
      uint8_t minutes = 23;
      uint8_t seconds = 59;

      WHEN("Static or dynamic")
	{
	  std::size_t size = (std::size_t)time_<config::static_, action::size>(nullptr, hours, minutes, seconds);

	  REQUIRE(size == 8);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(size)
	      {
		THEN("Prepare, write or reset")
		  {
		    end = time_<config::static_, action::prepare>(buffer, hours, minutes, seconds);

		    REQUIRE(end - begin == size);
		    REQUIRE(std::string(begin, end) == "05:23:59");
		  }
	      }
	    }
	}
    }

  WHEN("tm")
    {
      std::tm time;
      time.tm_hour = 4;
      time.tm_min = 34;
      time.tm_sec = 0;

      std::size_t size = (std::size_t)time_<config::static_, action::size>(nullptr, time);

      GIVEN("Size")
	{
	  GIVEN_A_BUFFER(size)
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
  WHEN("universal")
    {
      const char pad = ' ';

      WHEN("Static")
	{
	  WHEN("Left or right aligned")
	    {
	      std::size_t size = (std::size_t)universal_timezone<config::static_, align::left, pad, action::size>(nullptr, universal_timezone::GMT);

	      REQUIRE(size == 3);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare GMT")
		      {
			end = universal_timezone<config::static_, align::left, pad, action::prepare>(begin, universal_timezone::GMT);

			REQUIRE(end - begin == 3);
			REQUIRE(std::string(begin, end) == "GMT");
		      }

		    THEN("Prepare UT")
		      {
			end = universal_timezone<config::static_, align::left, pad, action::prepare>(begin, universal_timezone::UT);

			REQUIRE(end - begin == 2);
			REQUIRE(std::string(begin, end) == "UT");
		      }
		  }
		}
	    }
	}

      WHEN("Dynamic")
	{
	  std::size_t size = (std::size_t)universal_timezone<config::dynamic, align::left, pad, action::size>(nullptr, universal_timezone::UT);

	  REQUIRE(size == 3);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(3)
		{
		  WHEN("Left-aligned")
		    {
		      THEN("Prepare, write or reset")
			{
			  end = universal_timezone<config::dynamic, align::left, pad, action::prepare>(begin, universal_timezone::UT);

			  REQUIRE(end - begin == 3);
			  REQUIRE(std::string(begin, end) == "UT ");
			}
		    }

		  WHEN("Right-aligned")
		    {
		      THEN("Prepare, write or reset")
			{
			  end = universal_timezone<config::dynamic, align::right, pad, action::prepare>(begin, universal_timezone::UT);

			  REQUIRE(end - begin == 3);
			  REQUIRE(std::string(begin, end) == " UT");
			}
		    }

		  WHEN("GMT")
		    {
		      end = universal_timezone<config::dynamic, align::right, pad, action::prepare>(begin, universal_timezone::GMT);

		      REQUIRE(end - begin == 3);
		      REQUIRE(std::string(begin, end) == "GMT");
		    }
		}
	    }
	}
    }

  WHEN("universal_timezone_t")
    {
      typedef universal_timezone_t<config::dynamic, align::left, ' '> universal_timezone_type;

      universal_timezone_type timezone;

      std::size_t size = (std::size_t)timezone.handle<action::size>(nullptr);

      REQUIRE(size == 3);
    }

  WHEN("north american")
    {
      WHEN("Static or dynamic")
	{
	  std::size_t size = (std::size_t)north_american_timezone<config::static_, action::size>(nullptr, north_american_timezone::CST);

	  REQUIRE(size == 3);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(3)
		{
		  end = north_american_timezone<config::static_, action::prepare>(begin, north_american_timezone::CST);

		  REQUIRE(end - begin == 3);
		  REQUIRE(std::string(begin, end) == "CST");
		}
	    }
	}
    }

  WHEN("north_american_timezone_t")
    {
      typedef north_american_timezone_t<config::dynamic> north_american_timezone_type;

      north_american_timezone_type timezone;

      std::size_t size = (std::size_t)timezone.handle<action::size>(nullptr);

      REQUIRE(size == 3);
    }

  WHEN("military")
    {
      //see character

#define TST(C, I)							\
      REQUIRE(details::military_timezone_offset_2_letter<int8_t>((int8_t)(I)) == (C)); \
      REQUIRE(details::military_timezone_letter_2_offset(C) == (I))

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
    }

  WHEN("military_timezone_t")
  {
    typedef military_timezone_t<config::dynamic> military_timezone_type;

    military_timezone_type timezone;

    std::size_t size = (std::size_t)timezone.handle<action::size>(nullptr);

    REQUIRE(size == 1);
  }

  WHEN("differential")
    {
      WHEN("Static or dynamic")
	{
	  std::size_t size = (std::size_t)differential_timezone<config::static_, action::size>(nullptr, true, 12, 54);

	  REQUIRE(size == 5);

	  GIVEN("Size")
	    {
	      GIVEN_A_BUFFER(5)
		{
		  THEN("Prepare")
		    {
		      end = differential_timezone<config::static_, action::prepare>(begin, true, 14, 45);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "+1445");
		    }

		  THEN("Prepare")
		    {
		      end = differential_timezone<config::static_, action::prepare>(begin, false, 00, 05);

		      REQUIRE(end - begin == 5);
		      REQUIRE(std::string(begin, end) == "-0005");
		    }
		}
	    }
	}
    }

  WHEN("differential_timezone_t")
    {
      typedef differential_timezone_t<config::dynamic, uint8_t, uint8_t> differential_timezone_type;

      differential_timezone_type timezone;

      std::size_t size = (std::size_t)timezone.handle<action::size>(nullptr);

      REQUIRE(size == 5);
    }
}

SCENARIO("String", "[string]")
{
  const char * data = "Hello world!";
  const std::size_t length = std::strlen(data);
  const char pad = ' ';

  GIVEN("A string and a padding character")
    {
      const std::size_t max_length_ = 32;

      WHEN("Static")
	{
	  std::size_t max_length = length;

	  WHEN("Left or right aligned")
	    {
	      std::size_t size = (std::size_t)string<config::static_, align::left, pad, action::size>(nullptr, nullptr, 0, max_length);

	      REQUIRE(size == length);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = string<config::static_, align::left, pad, action::prepare>(begin, data, length, max_length);

			REQUIRE(end - begin == length);
			REQUIRE(std::string(begin, end) == data);
		      }
		  }
		}
	    }
	}

      WHEN("Dynamic")
	{
	  std::size_t max_length = max_length_;
	  std::size_t previous_length = 0;

	  WHEN("Left-aligned")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::left, pad, action::size>(nullptr, nullptr, length, max_length, previous_length);

	      REQUIRE(size == max_length);
	      REQUIRE(max_length == max_length_);
	      REQUIRE(previous_length == 0);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = string<config::dynamic, align::left, pad, action::prepare>(begin, "world!", 6, max_length, previous_length);

			REQUIRE(end - begin == max_length);
			REQUIRE(max_length == max_length_);
			REQUIRE(previous_length == 0);
			REQUIRE(std::string(begin, end) == std::string(max_length, pad));

			THEN("Write")
			  {
			    end = string<config::dynamic, align::left, pad, action::write>(begin, data, length, max_length, previous_length);

			    REQUIRE(end - begin == max_length);
			    REQUIRE(max_length == max_length_);
			    REQUIRE(previous_length == length);
			    REQUIRE(std::string(begin, end) == data + std::string(max_length - length, pad));

			    THEN("Reset")
			      {
				end = string<config::dynamic, align::left, pad, action::reset>(begin, nullptr, 0, max_length, previous_length);

				REQUIRE(end - begin == max_length);
				REQUIRE(max_length == max_length_);
				REQUIRE(previous_length == 0);
				REQUIRE(std::string(begin, end) == std::string(max_length, pad));
			      }
			  }
		      }
		  }
		}
	    }

	  WHEN("Right-aligned")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::right, pad, action::size>(nullptr, nullptr, 0, max_length, previous_length);

	      REQUIRE(size == max_length);
	      REQUIRE(max_length == max_length_);
	      REQUIRE(previous_length == 0);

	      GIVEN("Size")
		{
		  GIVEN_A_BUFFER(size)
		  {
		    THEN("Prepare")
		      {
			end = string<config::dynamic, align::right, pad, action::prepare>(begin, "world!", 6, max_length, previous_length);

			REQUIRE(end - begin == max_length);
			REQUIRE(max_length == max_length_);
			REQUIRE(previous_length == 0);
			REQUIRE(std::string(begin, end) == std::string(max_length, pad));

			THEN("Write")
			  {
			    end = string<config::dynamic, align::right, pad, action::write>(begin, data, length, max_length, previous_length);

			    REQUIRE(end - begin == max_length);
			    REQUIRE(max_length == max_length_);
			    REQUIRE(previous_length == length);
			    REQUIRE(std::string(begin, end) == std::string(max_length - length, pad) + data);

			    THEN("Reset")
			      {
				end = string<config::dynamic, align::right, pad, action::reset>(begin, nullptr, 0, max_length, previous_length);

				REQUIRE(end - begin == max_length);
				REQUIRE(max_length == max_length_);
				REQUIRE(previous_length == 0);
				REQUIRE(std::string(begin, end) == std::string(max_length, pad));
			      }
			  }
		      }
		  }
		}
	    }
	}

      WHEN("Manual fill")
	{
	  WHEN("Static")
	    {
	      std::size_t size = (std::size_t)string<config::static_, align::left, pad, action::size>(nullptr, nullptr, 0, length);

	      GIVEN_A_BUFFER(size)
		{
		  THEN("Prepare")
		    {
		      char * value = nullptr;
		      end = string<config::static_, align::left, pad, action::prepare>(buffer, &value, length, length);

		      (void)length;
		      REQUIRE(value != nullptr);
		      REQUIRE(end - begin == size);
		      std::memcpy(value, data, length);
		      REQUIRE(std::string(begin, end) == data);
		    }
		}
	    }

	  WHEN("Dynamic")
	    {
	      std::size_t size = (std::size_t)string<config::dynamic, align::left, pad, action::size>(nullptr, nullptr, 0, length);

	      GIVEN_A_BUFFER(size)
	      {
		THEN("Prepare")
		  {
		    char * value = nullptr;
		    end = string<config::dynamic, align::left, pad, action::prepare>(buffer, &value, length, length);
		    REQUIRE(value != nullptr);
		    REQUIRE(end - begin == size);
		    std::memcpy(value, data, length);
		    REQUIRE(std::string(begin, end) == data);
		  }
	      }
	    }
	}

      WHEN("string_t<config::static_>")
	{
	  typedef string_t<config::static_, align::left, ' '> string_type;

	  string_type string = string_type();
	  std::size_t size = (std::size_t)string.handle<action::size>(nullptr, nullptr, 32);

	  REQUIRE(size == 32);
	}

      WHEN("string_t<config::dynamic>")
	{
	  typedef string_t<config::dynamic, align::left, ' '> string_type;

	  string_type string = string_type();

	  std::size_t size = (std::size_t)string.handle<action::size>(nullptr, nullptr, 32);

	  REQUIRE(size == 32);
	}

      WHEN("string_t<config::dynamic, ..., true>")
	{
	  typedef string_t<config::dynamic, align::left, ' ', true> string_type;

	  string_type string = string_type();

	  std::size_t size = (std::size_t)string.handle<action::size>(nullptr, nullptr, 32);

	  REQUIRE(size == 32);
	}
    }
}
