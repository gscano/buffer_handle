#ifndef BUFFER_HANDLE_NOTHING_HPP
#define BUFFER_HANDLE_NOTHING_HPP

#include <buffer_handle/action.hpp> // action

namespace buffer_handle
{
  struct nothing_t
  {
    template<action Action>
    char * handle(char * buffer) const;
  };
};

#include <buffer_handle/nothing.hcp>

#endif/*BUFFER_HANDLE_NOTHING_HPP*/
