#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // std::cerr

#include <buffer_handle/character.hpp> // character()
#include <buffer_handle/number.hpp> // integral_number() two_digits_number()
#include <buffer_handle/string.hpp> // string()
#include <buffer_handle/time.hpp> // time_()
#include <buffer_handle/token.hpp> // dot() space()

using namespace buffer_handle;

template<config Config, std::size_t MaxDescriptionLength, action Action, typename Hours, typename Minutes, typename Temperature>
char * handle(char * buffer, Hours hours, Minutes minutes, int when, const char * description, Temperature temperature, char scale)
{
  buffer = string<config::static_, Action>(buffer, "At ", 3);
  buffer = time_<Config, Action>(buffer, hours, minutes);

  const char * tmp1 = ", the weather ";
  buffer = string<config::static_, Action>(buffer, tmp1, std::strlen(tmp1));

  if(when < 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "was", 3, 7);
    }
  else if(when == 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "is", 2, 7);
    }
  else
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "will be", 7, 7);
    }

  buffer = space<config::static_, Action>(buffer);

  int length = description == nullptr ? 0 : std::strlen(description);
  buffer = string<Config, align::right, ' ', Action>(buffer, description, length, MaxDescriptionLength);

  const char * tmp2 = " with a temperature of ";
  buffer = string<config::static_, Action>(buffer, tmp2, std::strlen(tmp2));

  buffer = two_digits_number<Config, ' ', Action>(buffer, temperature);
  buffer = string<config::static_, Action>(buffer, "°", std::strlen("°"));
  buffer = character<Config, Action>(buffer, scale);
  buffer = dot<config::static_, Action>(buffer);

  return buffer;
}

template<std::size_t MaxDescriptionLength>
void static_(int hours, int minutes, int when, const char * description, int temperature, char scale)
{
  std::cerr << "Static usage, ";

  std::size_t size = (std::size_t)handle<config::static_, MaxDescriptionLength, action::size>(nullptr, hours, minutes, when, description, temperature, scale);

  std::cerr << "size = " << size << ": ";

  char buffer[size] = {0};

  handle<config::static_, MaxDescriptionLength, action::prepare>(buffer, hours, minutes, when, description, temperature, scale);
  std::cerr << std::string(buffer, size) << std::endl;
}

void dynamic()
{
  std::cerr << "Dynamic usage, ";

  std::size_t size = (std::size_t)handle<config::dynamic, 10, action::size>(nullptr, 0, 0, 0, nullptr, 0, 'K');

  std::cerr << "size = " << size << ":" << std::endl << std::endl;

  char buffer[size] = {0};

  handle<config::dynamic, 10, action::prepare>(buffer, 0, 0, 0, "", 0, 'C');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;

  handle<config::dynamic, 10, action::write>(buffer, 9, 23, 0, "sunny", 23, 'C');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;

  handle<config::dynamic, 10, action::reset>(buffer, 2, 0, -1, "", 68, 'F');
  handle<config::dynamic, 10, action::write>(buffer, 23, 58, -1, "cloudy", 9, 'K');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;

  handle<config::dynamic, 10, action::reset>(buffer, 2, 0, -1, "", 68, 'F');
  handle<config::dynamic, 10, action::write>(buffer, 23, 58, 1, "freezing", 9, 'K');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;
}

int main()
{
  static_<10>(9, 23, 0, "sunny", 23, 'C');
  static_<10>(2, 0, -1, "cloudy", 68, 'F');
  static_<10>(23, 58, 1, "freezing", 9, 'K');

  std::cerr << std::endl;

  dynamic();

  return EXIT_SUCCESS;
}
