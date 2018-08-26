#ifndef HANDLE_BUFFER_BOOLEAN_HPP
#define HANDLE_BUFFER_BOOLEAN_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/align.hpp>
#include <buffer_handle/case.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, case_ Case, align Align, char Pad, action Action>
  char * boolean(char * buffer, bool value);
};

#include <buffer_handle/boolean.hcp>

#endif/*HANDLE_BUFFER_BOOLEAN_HPP*/
