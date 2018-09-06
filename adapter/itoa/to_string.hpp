#ifndef BUFFER_HANDLE_ADAPTER_ITOA_TO_STRING_HPP
#define BUFFER_HANDLE_ADAPTER_ITOA_TO_STRING_HPP

namespace buffer_handle
{
  namespace adapter
  {
    namespace itoa
    {
      struct to_string_t
      {
	template<typename I>
	char * fwd(char * buffer, I i) const;

	template<typename I>
	char * bwd(char * buffer, I i) const;
      };
    };
  };
};

#include <buffer_handle/adapter/itoa/to_string.hcp>

#endif/*BUFFER_HANDLE_ADAPTER_ITOA_TO_STRING_HPP*/
