#ifndef BUFFER_HANDLE_TIME_HPP
#define BUFFER_HANDLE_TIME_HPP

#include <ctime> // time_t tm
#include <cstdint> // uint8_t

#include <buffer_handle/action.hpp>
#include <buffer_handle/align.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, align Align, char Pad, class Itoa, action Action, typename MaxDigits = uint8_t>
  char * time_(char * buffer, time_t time, MaxDigits & max_digits, const Itoa & itoa = Itoa());

  template<config Config, action Action, typename Hours, typename Minutes>
  char * time_(char * buffer, Hours hours, Minutes minutes);

  template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
  char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

  template<config Config, action Action>
  char * time_(char * buffer, std::tm time);
};

#include <buffer_handle/time.hcp>

#endif/*BUFFER_HANDLE_TIME_HPP*/
