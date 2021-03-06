#ifndef BUFFER_HANDLE_HELPER_HPP
#define BUFFER_HANDLE_HELPER_HPP

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> // align
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  constexpr bool must_write(config Config, action Action);
  constexpr action write_when_reset(action value);
  constexpr action always_write(action value);

  template<align Align, bool UsePreviousLength, char Pad, typename Size>
  void reset(char * buffer, Size max_length, Size & previous_length);

  template<bool UsePreviousLength, char Pad, typename Size>
  void pad_left(char * begin, char * end, Size max_length, Size & previous_length);

  template<bool UsePreviousLength, char Pad, typename Size>
  void pad_right(char * begin, char * end, Size max_length, Size & previous_length);

  template<char Separator>
  struct character_separator_t
  {
    template<config Config, action Action>
    char * handle(char * buffer) const;
  };

  template<char Separator>
  struct character_and_space_separator_t
  {
    template<config Config, action Action>
    char * handle(char * buffer) const;
  };
};

#include <buffer_handle/helper.hcp>

#endif/*BUFFER_HANDLE_HELPER_HPP*/
