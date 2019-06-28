#ifndef BUFFER_HANDLE_TEST_HPP
#define BUFFER_HANDLE_TEST_HPP

#define GIVEN_A_BUFFER_(size, tsize)		\
  char buffer[size] = {0};			\
  char * begin = buffer;			\
  char * end = buffer;				\
  GIVEN("A " tsize " bytes buffer")

#define GIVEN_A_BUFFER(size) GIVEN_A_BUFFER_(size, #size)

#define FOR( desc )      INTERNAL_CATCH_DYNAMIC_SECTION( "      For: " << desc )
#define FIRST( desc )    INTERNAL_CATCH_DYNAMIC_SECTION( "    First: " << desc )

#endif/*BUFFER_HANDLE_TEST_HPP*/
