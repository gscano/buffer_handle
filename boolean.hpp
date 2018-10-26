#ifndef HANDLE_BUFFER_BOOLEAN_HPP
#define HANDLE_BUFFER_BOOLEAN_HPP

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> // align
#include <buffer_handle/case.hpp> // case
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, case_ Case, align Align, char Pad, action Action>
  char * boolean(char * buffer, bool value);

  template<config Config, action Action, char False = '0', char True = '1'>
  char * boolean(char * buffer, bool value);
};

#include <buffer_handle/boolean.hcp>

#endif/*HANDLE_BUFFER_BOOLEAN_HPP*/
