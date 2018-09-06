#ifndef BUFFER_HANDLE_HELPER_HPP
#define BUFFER_HANDLE_HELPER_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, action Action>
  constexpr bool must_write()
  {
    return (Config == config::static_ && Action == action::prepare)
      || (Config == config::dynamic && Action != action::size);
  }
};

#endif/*BUFFER_HANDLE_HELPER_HPP*/
