# MAKEFLAGS+=.silent

# CXX=clang++-3.8
CXX=clang
# CXX=g++
CXXFLAGS=-g -Wall # -Xclang -flto-visibility-public-std # -target x86_64-pc-windows-gnu

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:.cpp=.o)
# PROGS=test

# all: ${PROGS}

lzw: obj
	${CXX} ${CXXFLAGS} -o lzw ${OBJS}

test: obj
	${CXX} ${CXXFLAGS} -o test ${OBJS}

obj: $(OBJS)

unit-test: test
	./test

integrate-test: lzw
	pytest test_lzw.py

rebuild: clean all

clean:
	rm -rf *.o *.gch *.exe *.ilk *.pdb

.PHONY: all obj run_test rebuild clean
