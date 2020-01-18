-include config.mk

CXX ?= g++
CXXSTD ?= c++11
CXXFLAGS ?= -Wall -Wextra -Werror -MD -O0 -g --coverage -fno-inline
CATCH ?= .

all: test run-test example Makefile

run-test: test
	./$<

test: test.cpp Makefile
	$(CXX) $< -std=$(CXXSTD) $(CXXFLAGS) -I $(CATCH) -I ../ -o $@

ci-test:  test.cpp Makefile
	$(CXX) $< -std=$(CXXSTD) -Wall -Wextra -Werror -I $(CATCH) -I ../ -o $@

-include test.d

example: example.cpp Makefile
	g++ $< -g -std=$(CXXSTD) -O0 -I ../ -o $@

coverage: run-test
	lcov --capture --directory . --output-file coverage.data
	lcov --remove coverage.data "/usr/include/*" "6/*" "$(CATCH)*" -o coverage.info
	genhtml coverage.info -o $@

clean:
	rm -f test.d test.o test
	rm -rf .coverage coverage coverage.info test.gcda test.gcno
	rm -f example
