#ifndef BUFFER_HANDLE_CONTAINER_HPP
#define BUFFER_HANDLE_CONTAINER_HPP

#include <cstddef> // size_t

#include <buffer_handle/action.hpp>
#include <buffer_handle/config.hpp>

namespace buffer_handle
{
  template<config Config, align Align, char Pad, action Action,
	   class Iterator, class Element, class Separator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Element & element, Separator & separator);

  template<config Config, align Align, char Pad, class Element, class Separator>
  struct container_t
  {
  public:
    container_t(std::size_t max_length, const Element & element = Element(), const Separator & separator = Separator());

  protected:
    std::size_t max_length;

  protected:
    Element element;
    Separator separator;

  public:
    template<action Action, class Iterator>
    char * handle(char * buffer, const Iterator & begin, const Iterator & end);
  };
};

#include <buffer_handle/container.hcp>

#endif/*BUFFER_HANDLE_CONTAINER_HPP*/
