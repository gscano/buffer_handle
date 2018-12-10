#ifndef BUFFER_HANDLE_TOKEN_HPP
#define BUFFER_HANDLE_TOKEN_HPP

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, action Action>
  char * new_line(char * buffer);

  template<config Config, action Action>
  char * carriage_return(char * buffer);

  template<config Config, action Action>
  char * space(char * buffer);

  template<config Config, action Action>
  char * double_quote(char * buffer);

  template<config Config, action Action>
  char * single_quote(char * buffer);

  template<config Config, action Action>
  char * plus(char * buffer);

  template<config Config, action Action>
  char * comma(char * buffer);

  template<config Config, action Action>
  char * hyphen(char * buffer);

  template<config Config, action Action>
  char * minus(char * buffer);

  template<config Config, action Action>
  char * dot(char * buffer);

  template<config Config, action Action>
  char * slash(char * buffer);

  template<config Config, action Action>
  char * colon(char * buffer);

  template<config Config, action Action>
  char * semicolon(char * buffer);

  template<config Config, action Action>
  char * equal(char * buffer);

  template<config Config, action Action>
  char * opening_bracket(char * buffer);

  template<config Config, action Action>
  char * closing_bracket(char * buffer);
};

#include <buffer_handle/token.hcp>

#endif/*BUFFER_HANDLE_TOKEN_HPP*/
