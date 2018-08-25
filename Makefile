-include config.mk

CATCH ?= .

all: test

-include test.d

test.o: test.cpp Makefile
	g++ -c -MD $< -I $(CATCH) -I ../ -o $@

test: test.o Makefile
	g++ $< -o $@
	./$@

clean:
	rm -f test.d test.o test
