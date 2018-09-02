-include config.mk

CATCH ?= .

test: test.o Makefile
	g++ $< -o $@

-include test.d

test.o: test.cpp Makefile
	g++ -c -MD -g -O0 $< -I $(CATCH) -I $(ITOA) -I ../ -o $@

clean:
	rm -f test.d test.o test
