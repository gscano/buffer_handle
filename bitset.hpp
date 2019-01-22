#ifndef BUFFER_HANDLE_BITSET_HPP
#define BUFFER_HANDLE_BITSET_HPP

#include <type_traits> // is_integral

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/config.hpp> // config
#include <buffer_handle/container.hpp> // container_t

namespace buffer_handle
{
  template<class Bitset, action Action, class Separator>
  char * bitset(char * buffer, typename Bitset::value_type value, Separator & separator);

  template<config Config, align Align, char Pad, class Bitset, action Action, class Separator>
  char * bitset(char * buffer, typename Bitset::value_type value, std::size_t & max_length, Separator & separator);

  template<config Config, align Align, char Pad, class Bitset, action Action, class Separator>
  char * bitset(char * buffer, typename Bitset::value_type value, std::size_t & max_length, Separator & separator, std::size_t & previous_length);

  template<config Config, align Align, char Pad, class Bitset, bool IsLong = false>
  struct bitset_t
  {
    template<action Action, class Separator>
    char * handle(char * buffer, typename Bitset::value_type value, Separator & separator);
  };
};

#include <buffer_handle/bitset.hcp>

#endif/*BUFFER_HANDLE_BITSET_HPP*/
