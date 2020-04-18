# MAKEFLAGS+=.silent

SRC_DIR=src
INC_DIR=include
BUILD_DIR=build
TEST_DIR=tests

SRCS=$(wildcard ${SRC_DIR}/*.cpp)
OBJS=$(addprefix ${BUILD_DIR}/,$(notdir $(SRCS:.cpp=.o)))
TEST_PROGS=$(addprefix ${BUILD_DIR}/,$(patsubst %.cpp,%,$(notdir $(wildcard ${TEST_DIR}/*.cpp))))
PROGS=${BUILD_DIR}/lzw

# CXX=clang++-3.8
# CXX=clang
CXX=clang++
CXXFLAGS=-pg # -fprofile-arcs -ftest-coverage
# CXXFLAGS=-Ofast -Wall -Wextra -pedantic
# CXXFLAGS=-g -Wall -Wextra -pedantic
# -Xclang -flto-visibility-public-std # -target x86_64-pc-windows-gnu
# CXX=g++
# CXXFLAGS=-pg -fprofile-arcs -ftest-coverage
# CXXFLAGS=-g -Wall -Wextra -pedantic -Werror
# --std=c++11
CPPFLAGS=-I${INC_DIR}
AR=llvm-ar
ARFLAGS=crs

# TODO: out-of-source build

# TODO: use concatenate_source_file to concat entry files, then compile, so that we can save all the hassle of setting up obscure Makefile

# TODO: build static library

# TODO: using vanilla GNU Make has no easy way to automatically specify files' dependencies from included headers

# bin/bitarray.o: src/bitarray.cpp include/converter.h

# bin/bitmap.o: src/bitmap.cpp

# bin/bytes.o: src/bytes.cpp include/utils.h

all: ${PROGS}

rebuild: clean all

build-test: ${TEST_PROGS}

unit-test: build-test
	./runall.py ${TEST_PROGS}

integrate-test: ${BUILD_DIR}/lzw # ${PROGS}
	pytest test_lzw.py

${BUILD_DIR}/lzw: lzw.cpp ${BUILD_DIR}/liblzw.a
	${CXX} -o $@ $^ ${CXXFLAGS} ${CPPFLAGS}

${BUILD_DIR}/%: ${TEST_DIR}/%.cpp ${BUILD_DIR}/liblzw.a
	${CXX} -o $@ $^ ${CXXFLAGS} ${CPPFLAGS}

${BUILD_DIR}/liblzw.a: ${OBJS}
	${AR} ${ARFLAGS} $@ $^

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} -c -o $@ $< ${CXXFLAGS} ${CPPFLAGS}

# We have to specify prerequisite for str_dict and code_dict, because they rely on dict.tpp, a template header.

${BUILD_DIR}/str_dict.o: ${SRC_DIR}/str_dict.cpp ${INC_DIR}/dict.tpp ${INC_DIR}/set.tpp
	${CXX} -c -o $@ $< ${CXXFLAGS} ${CPPFLAGS}

${BUILD_DIR}/code_dict.o: ${SRC_DIR}/code_dict.cpp ${INC_DIR}/dict.tpp
	${CXX} -c -o $@ $< ${CXXFLAGS} ${CPPFLAGS}

# how to specify existence of directory, instead of regular file, as prerequisite
build:
	mkdir -p $@

cov:
	gcov ${SRCS}

prof:
	gprof ${PROGS}.exe gmon.out > prof_result

concat:
	concat lzw.cpp -I include -S src
	# g++ -Ofast -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw
	g++ -pg -Ofast -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw
	# g++ --std=c++11 -static-libstdc++ -Wall -Wextra -g concated.cpp -o build/lzw

clean:
	rm -rf ${BUILD_DIR}/*

.PHONY: all rebuild build-test unit-test integrate-test cov prof concat clean
