CXX=g++
CXXFLAGS=-g
PROGS=lzw

all: ${PROGS}

lzw: lzw.cpp

test:
	pytest test_lzw.py

clean:
	rm -f lzw

.PHONY: all test clean
