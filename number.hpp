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

  template<config Config, align Align, char Pad, action Action, class Itoa, typename I, typename MaxDigits = uint8_t>
  char * integral_number(char * buffer, I i, MaxDigits & max_digits, MaxDigits & previous_digits, const Itoa & itoa);

  template<config Config, align Align, char Pad, action Action, class Itoa, typename I, typename MaxDigits = uint8_t>
  char * integral_number(char * buffer, I i, MaxDigits & max_digits, const Itoa & itoa);

  template<config Config, align Align, char Pad, typename I, typename MaxDigits = uint8_t>
  struct integral_number_t
  {
  public:
    integral_number_t();

  protected:
    MaxDigits max_digits;

  public:
    template<action Action, class Itoa>
    char * handle(char * buffer, I value, const Itoa & itoa = Itoa());
  };

  template<config Config, align Align, char Pad, typename I, typename MaxDigits = uint8_t>
  struct long_integral_number_t : public integral_number_t<Config, Align, Pad, I, MaxDigits>
  {
  public:
    long_integral_number_t();

  protected:
    MaxDigits previous_digits;

  public:
    template<action Action, class Itoa>
    char * handle(char * buffer, I value, const Itoa & itoa = Itoa());
  };
};

#include <buffer_handle/number.hcp>

#endif/*BUFFER_HANDLE_NUMBER_HPP*/
