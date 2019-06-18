#ifndef BUFFER_HANDLE_NUMBER_HPP
#define BUFFER_HANDLE_NUMBER_HPP

#include <cstdint> // uint8_t

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> // align
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, char InsteadOfALeadingZero, action Action, typename I>
  char * two_digits_number(char * buffer, I i);

  template<config Config, action Action, typename I>
  char * four_digits_number(char * buffer, I i);

  template<action Action, class Itoa, typename I>
  char * integral_number(char * buffer, I i, const Itoa & itoa = Itoa());

  template<config Config, align Align, char Pad, action Action, class Itoa, typename I, typename Digits = uint8_t>
  char * integral_number(char * buffer, I i, Digits & max_digits, Digits & previous_digits, const Itoa & itoa = Itoa());

  template<config Config, align Align, char Pad, action Action, class Itoa, typename I, typename Digits = uint8_t>
  char * integral_number(char * buffer, I i, Digits & max_digits, const Itoa & itoa = Itoa());

  template<config Config, align Align, char Pad, typename I, typename Digits = uint8_t, bool IsLong = false>
  struct integral_number_t
  {
    template<action Action, class Itoa>
    char * handle(char * buffer, I value, const Itoa & itoa = Itoa());
  };
};

#include <buffer_handle/number.hcp>

#endif/*BUFFER_HANDLE_NUMBER_HPP*/
