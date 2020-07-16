#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // std::cout

#include <buffer_handle/number.hpp> // number()
#include <buffer_handle/string.hpp> // string()
#include <buffer_handle/token.hpp> // carriage_return(), new_line(), space()

#include <buffer_handle/adapter/itoa/to_string.hpp> // to_string_t

using namespace buffer_handle;

template<config Config,
	 action Action>
char * handle(char * buffer, unsigned short code, const char * reason, std::size_t length, const char * type, const char * encoding)
{ 
  unsigned char code_max_digits = 3;
  std::size_t reason_max_length = 10;
  std::size_t max_length_digits = 4;
  std::size_t type_max_length = 10;
  std::size_t max_encoding_length = 10;
    
  buffer = string<config::static_, Action>(buffer, "HTTP/1.1 ");
  buffer = integral_number<Config, align::right, ' ', Action, adapter::itoa::to_string_t>
    (buffer, code, code_max_digits);
  buffer = space<config::static_, Action>(buffer);
  buffer = string<Config, align::right, ' ', Action>
    (buffer, reason, std::strlen(reason), reason_max_length);

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Length: ");
  buffer = integral_number<Config, align::right, ' ', Action, adapter::itoa::to_string_t>
    (buffer, length, max_length_digits);

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Type: ");
  buffer = string<Config, align::right, ' ', Action>
    (buffer, type, std::strlen(type), type_max_length);
  buffer = string<config::static_, Action>(buffer, "; charset=UTF-8");

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Encoding: ");
  buffer = string<Config, align::right, ' ', Action>
    (buffer, encoding, std::strlen(encoding), max_encoding_length);

  return buffer;
}

int main()
{
  const char * ok = "OK";
  const char * not_found = "Not Found";

  const char * html = " text/html";
  const char * plain = "text/plain";

  const char * identity = "identity";
  const char * gzip = "gzip";

  char * end = handle<config::dynamic, action::size>(nullptr, 999, not_found, 9999, plain, identity);
  const std::size_t size = (std::size_t)end;

  std::cout << "size: " << size << " bytes." << std::endl;

  char buffer[size + 1] = {0};

  std::cout << "prepared:" << std::endl << "==========" << std::endl;
  end = handle<config::dynamic, action::prepare>(buffer, 999, "", 9999, "", "");
  std::cout << buffer << std::endl << "==========" << std::endl;;
  
  std::cout << "write:" << std::endl << "==========" << std::endl;
  end = handle<config::dynamic, action::write>(buffer, 200, ok, 2783, html, gzip);
  std::cout << buffer << std::endl << "==========" << std::endl;;
  
  std::cout << "reset:" << std::endl << "==========" << std::endl;
  end = handle<config::dynamic, action::write>(buffer, 0, ok, 0, "", "");
  std::cout << buffer << std::endl << "==========" << std::endl;;
  
  std::cout << "write:" << std::endl << "==========" << std::endl;
  end = handle<config::dynamic, action::write>(buffer, 404, not_found, 212, plain, identity);
  std::cout << buffer << std::endl << "==========" << std::endl;;
  
  return EXIT_SUCCESS;
}
