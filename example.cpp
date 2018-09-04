#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // std::cerr

#include <buffer_handle/character.hpp>
#include <buffer_handle/number.hpp>
#include <buffer_handle/string.hpp>
#include <buffer_handle/time.hpp>
#include <buffer_handle/token.hpp>

using namespace buffer_handle;

template<config Config, action Action, std::size_t MaxDescriptionLength>
char * handle(char * buffer, int hours, int minutes, int when, const char * description, int temperature, char degree)
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

  buffer = space<Config, Action>(buffer);

  int length = description == nullptr ? 0 : std::strlen(description);
  buffer = string<Config, align::right, ' ', Action>(buffer, description, length, MaxDescriptionLength);

  const char * tmp2 = " with a temperature of ";
  buffer = string<config::static_, Action>(buffer, tmp2, std::strlen(tmp2));

  buffer = two_digits_number<Config, ' ', Action>(buffer, temperature);
  buffer = string<config::static_, Action>(buffer, "°", std::strlen("°"));
  buffer = character<Config, Action>(buffer, degree);
  buffer = dot<config::static_, Action>(buffer);

  return buffer;
}

template<std::size_t MaxDescriptionLength>
void static_(int hours, int minutes, int when, const char * description, int temperature, char degree)
{
  std::cerr << "Static usage, ";

  std::size_t size = (std::size_t)handle<config::static_, action::size, MaxDescriptionLength>(nullptr, hours, minutes, when, description, temperature, degree);

  std::cerr << "size = " << size << ": ";

  char buffer[size] = {0};

  handle<config::static_, action::prepare, MaxDescriptionLength>(buffer, hours, minutes, when, description, temperature, degree);
  std::cerr << std::string(buffer, size) << std::endl;
}

void dynamic()
{
  std::cerr << "Dynamic usage, ";

  std::size_t size = (std::size_t)handle<config::dynamic, action::size, 10>(nullptr, 0, 0, 0, nullptr, 0, 'K');

  std::cerr << "size = " << size << ":" << std::endl << std::endl;

  char buffer[size] = {0};

  handle<config::dynamic, action::prepare, 10>(buffer, 9, 23, 0, "sunny", 23, 'C');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;

  handle<config::dynamic, action::write, 10>(buffer, 2, 0, -1, "cloudy", 68, 'F');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;

  handle<config::dynamic, action::reset, 10>(buffer, 23, 58, 1, "freezing", 99, 'K');
  std::cerr << "\t" << std::string(buffer, size) << std::endl;
}

int main()
{
  static_<10>(9, 23, 0, "sunny", 23, 'C');
  static_<10>(2, 0, -1, "cloudy", 68, 'F');
  static_<10>(23, 58, 1, "freezing", 99, 'K');

  std::cerr << std::endl;

  dynamic();

  return EXIT_SUCCESS;
}
