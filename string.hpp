#ifndef BUFFER_HANDLE_STRING_HPP
#define BUFFER_HANDLE_STRING_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/align.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, action Action>
  char * string(char * buffer, const char * value, std::size_t length);

  template<config Config, align Align, char Pad, action Action>
  char * string(char * buffer, const char * value, std::size_t length, std::size_t & max_length);

  template<config Config, align Align, char Pad>
  struct string_t
  {
  public:
    string_t(const char * value = nullptr, std::size_t length = 0);

  public:
    const char * value;
    std::size_t length;

  protected:
    std::size_t max_length;

  public:
    template<action Action>
    char * handle(char * buffer);
  };
};

#include <buffer_handle/string.hcp>

#endif/*BUFFER_HANDLE_STRING_HPP*/
