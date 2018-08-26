#ifndef BUFFER_HANDLE_ADAPTER_ITOA_HPP
#define BUFFER_HANDLE_ADAPTER_ITOA_HPP

namespace buffer_handle
{
  namespace adapter
  {
    struct itoa
    {
      template<typename I>
      char * fwd(char * buffer, I i) const;

      template<typename I>
      char * bwd(char * buffer, I i) const;
    };
  };
};

#include <buffer_handle/adapter/itoa.hcp>

#endif/*BUFFER_HANDLE_ADAPTER_ITOA_HPP*/
