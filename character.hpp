#ifndef BUFFER_HANDLE_CHARACTER_HPP
#define BUFFER_HANDLE_CHARACTER_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, action Action>
  char * character(char * buffer, char c);
};

#include <buffer_handle/character.hcp>

#endif/*BUFFER_HANDLE_CHARACTER_HPP*/
