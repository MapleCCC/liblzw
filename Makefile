# TODO: out-of-source build
# TODO: use concatenate_source_file to concat entry files, then compile, so that we can save all the hassle of setting up obscure Makefile
# TODO: build static library
# TODO: using vanilla GNU Make has no easy way to automatically specify files' dependencies from included headers

MAKEFLAGS += .silent

SRC_DIR=src
INC_DIR=include
BUILD_DIR=build
TEST_DIR=tests
SCRIPTS_DIR=scripts

SRCS=$(wildcard ${SRC_DIR}/*.cpp)
INCS=$(wildcard ${INC_DIR}/*.h)
TEST_PROGS=$(addprefix ${BUILD_DIR}/,$(patsubst %.cpp,%,$(notdir $(wildcard ${TEST_DIR}/*.cpp))))

all: build/lzw

fast: lzw.cpp
	mkdir -p build
	g++ -Ofast -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw

build/lzw: lzw.cpp
	mkdir -p build
	# g++ -Ofast -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw
	# g++ -pg -g -Ofast -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw
	# g++ -pg -g -static-libstdc++ --std=c++11 -Wall -Wextra concated.cpp -o build/lzw
	g++ --std=c++11 -static-libstdc++ -Wall -Wextra -g concated.cpp -o build/lzw

lzw.cpp: main.cpp ${SRCS} ${INCS}
	concat main.cpp -o lzw.cpp -I include -S src
	clang-format -i -style=file lzw.cpp

rebuild: clean all

test: unit-test integrate-test

build-test: ${TEST_PROGS}

# FIXME
unit-test:
# unit-test: build-test
	# ${SCRIPTS_DIR}/runall.py ${TEST_PROGS}

integrate-test: build/lzw
	pytest test_integrate.py

cov:
	# gcov ${SRCS}

prof:
	# gprof ${PROGS}.exe gmon.out > prof_result
	# gprof -l ${PROGS}.exe gmon.out > prof_result
	# code prof_result

reformat:
	# WARNING: don't use globstar like "echo **/*.cpp **/*.c **/*.h" because it has some limitations and pitfalls, like nullglob problem and it can't match current directory
	find ${SRC_DIR} ${INC_DIR} ${TEST_DIR} -type f -regextype posix-extended -regex ".*\.(c|cpp|h)" | xargs clang-format -i -style=file

compare-branch:
	git diff master assignment --compact-summary

todo:
	# ripgrep automatically skip undesired files and directories. Save the effort to manually exclude.
	rg -ri TODO

fixme:
	# ripgrep automatically skip undesired files and directories. Save the effort to manually exclude.
	rg -ri FIXME

clean:
	rm -rf build

pdf:
	# TODO: use the command line Typora's builtin PDF export functionality usees to invoke pandoc under the hood. Use the style from Typora.
	pandoc -s -o README.assignment.pdf README.assignment.md

eqn:
	eqn2imgurl README.raw.md -o README.md

release:


.PHONY: all rebuild test build-test unit-test integrate-test cov prof
.PHONY: reformat compare-branch todo fixme clean pdf eqn release
