-include config.mk

CATCH ?= .

all: test test-run

-include test.d

test.o: test.cpp Makefile
	g++ -c -MD $< -I $(CATCH) -I ../ -o $@

test: test.o Makefile
	g++ $< -o $@

test-run: test
	./test

clean:
	rm -f test.d test.o test
