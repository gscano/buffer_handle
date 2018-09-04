#ifndef BUFFER_HANDLE_CONTAINER_HPP
#define BUFFER_HANDLE_CONTAINER_HPP

#include <buffer_handle/action.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, align Align, char Pad, action Action,
	   class Iterator, class Handler, class Separator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Handler & handler, Separator & separator);

  template<config Config, align Align, char Pad, class Handler, class Separator>
  struct container_t
  {
  public:
    container_t(std::size_t max_length, const Handler & handler = Handler(), const Separator & separator = Separator());

  protected:
    std::size_t max_length;

  protected:
    Handler handler;
    Separator separator;

  public:
    template<action Action, class Iterator>
    char * handle(char * buffer, const Iterator & begin, const Iterator & end);
  };
};

#include <buffer_handle/container.hcp>

#endif/*BUFFER_HANDLE_CONTAINER_HPP*/
