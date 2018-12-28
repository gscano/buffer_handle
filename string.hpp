#ifndef BUFFER_HANDLE_STRING_HPP
#define BUFFER_HANDLE_STRING_HPP

#include <cstddef> // size_t
#include <type_traits> // enable_if

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> //align
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, action Action>
  char * string(char * buffer, const char * value, std::size_t length);

  template<config Config, action Action>
  char * string(char * buffer, char ** value, std::size_t length);

  template<config Config, action Action>
  char * string(char * buffer, const char * value);

  template<config Config, align Align, char Pad, action Action>
  char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length, std::size_t & previous_length);

  template<config Config, align Align, char Pad, action Action>
  char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length);

  template<config Config, align Align, char Pad, action Action>
  char * string(char * buffer, char ** value, std::size_t length, std::size_t max_length);

  template<config Config, align Align, char Pad, bool IsLong = false>
  struct string_t
  {
    template<action Action>
    char * handle(char * buffer, const char * value, std::size_t length);
  };
};

#include <buffer_handle/string.hcp>

#endif/*BUFFER_HANDLE_STRING_HPP*/
