-include config.mk

CATCH ?= .
ITOA  ?= .

all: test run-test Makefile

run-test: test Makefile
	./$<

test: test.o Makefile
	g++ $< -o $@

test.o: test.cpp Makefile
	g++ -c -MD -g -O0 $< -I $(CATCH) -I $(ITOA) -I ../ -o $@

-include test.d

clean:
	rm -f test.d test.o test
