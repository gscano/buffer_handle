-include config.mk

CXX ?= g++
CXXSTD ?= c++11
CXXFLAGS ?= -Wall -Wextra -Werror -MD -O0 -g -fno-inline

CATCH ?= .
BOOST ?= .

all: test run-test example Makefile

run-test: test
	./$<

test: test.cpp Makefile
	$(CXX) $< --coverage -std=$(CXXSTD) $(CXXFLAGS) -I $(CATCH) -I $(BOOST) -I ../ -o $@

-include text-example.d
-include html-example.d

example: text-example http-example
%-example: %-example.cpp Makefile
	g++ $< -std=$(CXXSTD) $(CXXFLAGS) -I $(BOOST) -I ../ -o $@

coverage: run-test
	lcov --capture --directory . --output-file coverage.data
	lcov --remove coverage.data "/usr/include/*" "6/*" "$(CATCH)*" -o coverage.info
	genhtml coverage.info -o $@

clean:
	rm -f test.d test.o test
	rm -rf .coverage coverage coverage.info test.gcda test.gcno
	rm -f text-example http-example
