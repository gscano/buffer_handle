#ifndef BUFFER_HANDLE_NUMBER_HPP
#define BUFFER_HANDLE_NUMBER_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/align.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, char InsteadOfALeadingZero, action Action, typename I>
  char * two_digits_number(char * buffer, I i);

  template<config Config, action Action, typename I>
  char * four_digits_number(char * buffer, I i);

  template<config Config, align Align, char Pad, class Itoa, action Action, typename I>
  char * integral_number(char * buffer, I i, uint8_t & max_digits, const Itoa & itoa = Itoa());

  template<config Config, align Align, char Pad, class Itoa, typename I>
  struct integral_number_t
  {
  public:
    integral_number_t(I value = I());

  public:
    I value;

  protected:
    uint8_t max_digits;

  public:
    template<action Action>
    char * handle(char * buffer, const Itoa & itoa = Itoa());
  };
};

#include <buffer_handle/number.hcp>

#endif/*BUFFER_HANDLE_NUMBER_HPP*/
