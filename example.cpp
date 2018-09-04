#include <cstdlib> // EXIT_SUCCESS

#include <buffer_handle/string.hpp>
#include <buffer_handle/time.hpp>

template<action Action>
char * handle(char * buffer, int hours, int minutes)
{
  using namespace buffer_handle;

  buffer = string<config::static_, Action>(buffer, "At ", 3);
  buffer = time_<config::dynamic, Action>(buffer, hours, minutes);

  return buffer;
}

int main()
{
  return EXIT_SUCCESS;
}
