#ifndef BUFFER_HANDLE_TIME_HPP
#define BUFFER_HANDLE_TIME_HPP

#include <ctime> // time_t struct tm

#include <buffer_handle/action.hpp>
#include <buffer_handle/align.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, align Align, char Pad, class Itoa, action Action>
  char * time_(char * buffer, time_t time, uint8_t & max_digits, const Itoa & itoa = Itoa());

  template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
  char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

  template<config Config, action Action>
  char * time_(char * buffer, struct tm time);
};

#include <buffer_handle/time.hcp>

#endif/*BUFFER_HANDLE_TIME_HPP*/
