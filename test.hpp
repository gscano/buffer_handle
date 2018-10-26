#ifndef BUFFER_HANDLE_TEST_HPP
#define BUFFER_HANDLE_TEST_HPP

#define GIVEN_A_BUFFER(size)			\
  char buffer[size] = {0};			\
  char * begin = buffer;			\
  char * end = buffer;				\
  GIVEN("A buffer")

#endif/*BUFFER_HANDLE_TEST_HPP*/
