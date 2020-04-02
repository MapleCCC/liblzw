CC=gcc
CFLAGS=-g
PROGS=lzw

all: ${PROGS}

lzw: lzw.c

test:
	pytest test_lzw.py

clean:
	rm -f lzw

.PHONY: all test clean
