-include config.mk

CXX ?= g++
CXXSTD ?= c++11
CXXFLAGS ?= -Wall -Wextra -Werror -MD -O0 -g -fno-inline

CATCH ?= .
BOOST ?= .

all: test run-test example Makefile

run-test: test
	./$<

-include test.d

test: test.cpp
	$(CXX) $< --coverage -std=$(CXXSTD) $(CXXFLAGS) -I $(CATCH) -I $(BOOST) -I ../ -o $@

-include text-example.d
-include html-example.d

example: text-example http-example
%-example: %-example.cpp Makefile
	g++ $< -std=$(CXXSTD) $(CXXFLAGS) -I $(BOOST) -I ../ -o $@

coverage.gcda: run-test

coverage.data: coverage.gcda
	lcov --capture --directory . --output-file coverage.data

coverage.info: coverage.data
	lcov --remove coverage.data "/usr/include/*" "6/*" $(CATCH)"/*" -o coverage.info

coverage: coverage.info
	genhtml coverage.info -o $@

clean:
	rm -f test.d test.o test
	rm -rf .coverage coverage.data coverage coverage.info test.gcda test.gcno
	rm -f text-example http-example
