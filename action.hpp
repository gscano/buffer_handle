#ifndef BUFFER_HANDLE_ACTION_HPP
#define BUFFER_HANDLE_ACTION_HPP

namespace buffer_handle
{
  enum class action : uint8_t
  {
    size,
      prepare,
      write,
      reset
      };
};

#endif/*BUFFER_HANDLE_ACTION_HPP*/
