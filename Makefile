# MAKEFLAGS+=.silent

# CXX=clang++-3.8
# CXX=clang++
# CXX=clang
CXX=g++
CXXFLAGS=-g --std=c++11 # -Wall # -Xclang -flto-visibility-public-std # -target x86_64-pc-windows-gnu

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:.cpp=.o)
# TODO: out-of-source build
# OBJS=$(OBJS:src/=build/)
# PROGS=test

# TODO: use concatenate_source_file to concat entry files, then compile, so that we can save all the hassle of setting up obscure Makefile

# all: ${PROGS}

lzw: obj
	${CXX} ${CXXFLAGS} -o lzw ${OBJS}

test: obj
	${CXX} ${CXXFLAGS} -o test ${OBJS}

obj: $(OBJS) src/code_dict.o src/str_dict.o

src/code_dict.o: src/code_dict.cpp src/dict.h

src/str_dict.o: src/str_dict.cpp src/dict.h

unit-test: test
	./test

integrate-test: lzw
	pytest test_lzw.py

rebuild: clean all

clean:
	rm -rf src/*.o src/*.gch *.exe *.ilk *.pdb

.PHONY: all obj unit-test integrate-test rebuild clean
