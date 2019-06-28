-include config.mk

CATCH ?= .

all: test run-test example Makefile

run-test: test
	./$<

test: test.cpp Makefile
	g++ $< -std=c++11 -Wall -Wextra -Werror -MD -O0 -g --coverage -fno-inline -I $(CATCH) -I ../ -o $@

-include test.d

example: example.cpp Makefile
	g++ $< -g -std=c++11 -O0 -I ../ -o $@

coverage: run-test
	lcov --capture --directory . --output-file coverage.data
	lcov --remove coverage.data "/usr/include/*" "6/*" "$(CATCH)*" -o coverage.info
	genhtml coverage.info -o $@

clean:
	rm -f test.d test.o test
	rm -rf .coverage coverage coverage.info test.gcda test.gcno
	rm -f example
