#ifndef BUFFER_HANDLE_TEST_HPP
#define BUFFER_HANDLE_TEST_HPP

#include <alloca.h>

#define GIVEN_A_BUFFER_(size, tsize)			\
  char * buffer = (char *)alloca(size);			\
  std::memset(buffer, 0, size);				\
  char * begin = buffer;				\
  char * end = buffer + size - 1;			\
  GIVEN("A " tsize " bytes buffer")

#define GIVEN_A_BUFFER(size) GIVEN_A_BUFFER_(size, #size)

#define FOR( desc )      INTERNAL_CATCH_DYNAMIC_SECTION( "      For: " << desc )
#define FIRST( desc )    INTERNAL_CATCH_DYNAMIC_SECTION( "    First: " << desc )

#endif/*BUFFER_HANDLE_TEST_HPP*/
