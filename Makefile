-include config.mk

CATCH ?= .

all: test

-include test.d

test.o: test.cpp
	g++ -c -MD $< -I $(CATCH) -I ../ -o $@

test: test.o
	g++ $< -o $@
	./$@

clean:
	rm -f test.o test
