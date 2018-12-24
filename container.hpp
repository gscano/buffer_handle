#ifndef BUFFER_HANDLE_CONTAINER_HPP
#define BUFFER_HANDLE_CONTAINER_HPP

#include <cstddef> // size_t

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, align Align, char Pad, action Action,
	   class Handler, class Separator, class Iterator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Handler & handler, Separator & separator);

  template<config Config, align Align, char Pad, action Action,
	   class Handler, class Separator, class Iterator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Handler & handler, Separator & separator, std::size_t & previous_length);

  template<config Config, align Align, char Pad, bool IsLong = false>
  struct container_t
  {
    void set_max_length(std::size_t length);

    template<action Action, class Iterator, class Handler, class Separator>
    char * handle(char * buffer, const Iterator & begin, const Iterator & end, Handler & handler, Separator & separator);
  };
};

#include <buffer_handle/container.hcp>

#endif/*BUFFER_HANDLE_CONTAINER_HPP*/
