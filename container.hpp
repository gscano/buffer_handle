#ifndef BUFFER_HANDLE_CONTAINER_HPP
#define BUFFER_HANDLE_CONTAINER_HPP

#include <cstddef> // size_t

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, align Align, char Pad, action Action,
	   class Element, class Separator, class Iterator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Element & element, Separator & separator);

  template<config Config, align Align, char Pad, action Action,
	   class Element, class Separator, class Iterator>
  char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		   Element & element, Separator & separator, std::size_t & previous_length);

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

  template<config Config, align Align, char Pad, class Element, class Separator>
  struct long_container_t : public container_t<Config, Align, Pad, Element, Separator>
  {
  public:
    long_container_t(std::size_t max_length, const Element & element = Element(), const Separator & separator = Separator());

  protected:
    std::size_t previous_length;

  public:
    template<action Action, class Iterator>
    char * handle(char * buffer, const Iterator & begin, const Iterator & end);
  };
};

#include <buffer_handle/container.hcp>

#endif/*BUFFER_HANDLE_CONTAINER_HPP*/
