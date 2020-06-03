# LZW Compressor

[![License](https://img.shields.io/github/license/MapleCCC/liblzw?color=00BFFF)](http://www.wtfpl.net/)
[![travisci](https://www.travis-ci.com/MapleCCC/liblzw.svg?branch=master)](https://travis-ci.com/MapleCCC/liblzw)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/MapleCCC/liblzw)](https://github.com/MapleCCC/liblzw/releases/latest)
[![Semantic release](https://img.shields.io/badge/%20%20%F0%9F%93%A6%F0%9F%9A%80-semantic--release-e10079.svg)](https://github.com/semantic-release/semantic-release)
[![LOC](https://sloc.xyz/github/MapleCCC/liblzw)](https://sloc.xyz/github/MapleCCC/liblzw)

## Table of Content

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Overview](#overview)
- [Installation](#installation)
- [Build](#build)
- [Usage](#usage)
- [Development](#development)
- [Test](#test)
- [Algorithmic Analysis](#algorithmic-analysis)
  - [Complexity Analysis](#complexity-analysis)
    - [Compression Algorithm Complexity](#compression-algorithm-complexity)
    - [Decompression Algorithm Complexity](#decompression-algorithm-complexity)
  - [Performance Analysis](#performance-analysis)
    - [Rationale](#rationale)
    - [Implementation Scheme](#implementation-scheme)
- [Hacks and Tricks](#hacks-and-tricks)
  - [Stream Style IO](#stream-style-io)
  - [Reserve Capacity to Reduce Resizing Cost](#reserve-capacity-to-reduce-resizing-cost)
  - [Cached Incremental Hash State to Reduce Recomputation Cost](#cached-incremental-hash-state-to-reduce-recomputation-cost)
  - [Immutable Data Structure to Reduce Space Cost](#immutable-data-structure-to-reduce-space-cost)
- [License](#license)
- [TODO](#todo)
- [TODO](#todo-1)
- [Archive](#archive)
- [TIL](#til)
- [CHANGELOG](#changelog)
- [Changelog](#changelog)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Overview

LZW is an archive format that utilizes power of LZW compression algorithm. LZW compression algorithm is a dictionary-based loseless algorithm. It's an old algorithm suitable for beginner to practice.

Internal algorithm processes byte data. So it's applicable to any file types, besides text file. Although it may not be able to achieve substantial compression rate for some file types that are already compressed efficiently, such as PDF files and MP4 files. It treats data as byte stream, unaware of the text-level pattern, which makes it less compression-efficient compared to other more advanced compression algorithms.

LZW compression algorithm is dynamic. It doesn't collect data statistics before hand. Instead, it learns the data pattern while conducting the compression, building a code table on the fly. The compression ratio approaches maximum after enough time. The algorithmic complexity is strictly linear to the size of the text. [A more in-depth algorithmic analysis can be found in the following sections.](#algorithmic-analysis)

An alternative implementation that utilizes more efficient self-made customized `bitmap`, `dict` and `set` to replace C++ builtin general-purpose `std::bitset`, `std::unordered_map` and `std::set` can be found in the branch [`assignment`](https://github.com/MapleCCC/liblzw/tree/assignment). Future enhancement includes customized `hash` functions to replace builtin general-purpose `std::hash`.

## Installation

Pre-built binaries (Windows only) are available on the [Releases](https://github.com/MapleCCC/liblzw/releases/latest) page. Release version conforms to the [semantic versioning convention](https://semver.org/).

## Build

Prerequisites: Git, Python3.6+, `pip`, and a modern C++ compiler: `g++` or `cl`.

```bash
$ git clone https://github.com/MapleCCC/liblzw.git

$ cd liblzw

# Install basic build requirements
$ pip install -r requirements

# if g++ is available:
$ make
# Compiled executable is in build/lzw

# if speed is desirable:
$ make fast

# if on Windows platform and cl.exe is available:
$ mkdir build && cl /Fe"build/lzw.exe" lzw.cpp
# Compiled executable is in build/lzw.exe

# if speed is desirable:
# compile in release mode
$ mkdir build && cl /D "NDEBUG" /O2 /Fe"build/lzw.exe" lzw.cpp
```

Alternatively, you can build as a static library for embedding in other applications.

```bash
# Build static library
$ make build-lib
# Compiled library is in build/liblzw.a

# Build dynamic library
[...]
```

## Usage

```bash
# Get Help Message
$ lzw --help
'''
Usage:

# Compression
$ lzw compress [-o|--output <ARCHIVE>] <FILES>...

# Decompression
$ lzw decompress <ARCHIVE>
'''
```

## Development

Contribution is welcome. When commiting new code, make sure to apply format specified in `.clang-format` config file. Also remember to add `scripts/pre-commit.py` to `.git/hooks/pre-commit` as pre-commit hook script.

Prerequisites: Git, Python3.6+, `pip`, `npm`, and a modern C++ compiler: `g++` or `cl`.

```bash
# Clone the repository to local environment
$ git clone https://github.com/MapleCCC/liblzw.git

$ cd liblzw

# Install basic build requirements
$ pip install -r requirements.txt

# Install dev requirements
$ pip install -r requirements-dev.txt
$ npm install

# Install pre-commit hook script
$ make install-pre-commit-hook-script
```

The pre-commit hook script basically does four things:

1. Format staged C/C++ code

2. Format staged Python code

3. Transform `LaTeX` math equation in `README.raw.md` to image url in `README.md`

4. Append content of `TODO.md` and `CHANGELOG.md` to `README.md`

5. Generate TOC for `README.md`

Besides relying on the pre-commit hook script, you can manually format code and transform math equations in `README.md`.

```bash
# Format C/C++ and Python code under the working directory
$ make reformat

# Transform LaTeX math equations to image urls in README.md
$ make transform-eqn
```

The advantages of pre-commit hook script, compared to manual triggering scripts, is that it's convenient and un-disruptive, as it only introduces changes to staged files, instead of to all the files in the repo.

## Test

- Unit test

  ```bash
  $ make unit-test
  ```

- Integrate test

  Python3.6+, Pytest and Hypothesis are needed for integrate test

  ```bash
  # You can optionally create a virtual environment for isolation purpose
  $ python -m virtualenv .venv
  $ source .venv/Scripts/activate

  $ python -m pip install -U pytest, hypothesis
  $ make integrate-test
  # It makes very thorough testing. It currently take about ten to twenty seconds.
  ```

## Algorithmic Analysis

### Complexity Analysis

Before talking about performance, we first conduct some complexity analysis. This help us understand the internal structure of the problem, and which and how various parts of the task contribute to the performance hotspots.

#### Compression Algorithm Complexity

LZW algorithm treats data as binary stream, regardless of its text encoding. During the algorithm process, a running word is maintained. Before algorithm starts, the running word is empty. Every new byte consumed by the algorithm would be appended to the running word. The running word would be reset to some value under some circumstances.

For each byte consumed by the algorithm, deciding upon whether the running word is in the code dict, the algorithm has **two** branches to go.

If the new running word formed by appending the new byte at the end is in the code dict, the algorithm proceeds to [![A](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A) branch, which basically do nothing, and continue to next iteration. On the other hand, if the new running word constituted by the new byte is not in the code dict, the algorithm goes to [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) branch.

In the [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) branch, three things happen. First, the code in the code dict corresponding to the old running word (before the new byte is appended) is emitted. Then the new running word is entered into the code dict as a new string, and assigned a new code, ready to use in the future. Lastly, the running word is reset to contain only the new byte. The algorithm then proceeds to the next iteration.

The cost model for these two branches are respectively:

[![
C_A = C(\mathrm{str.copy})
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(%5Cmathrm{str.copy}))

[![
C_B = C(\mathrm{dict.lookup}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy})
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))

Suppose the source text byte length is [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). Among the [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) bytes consumed by the algorithm, there are [![M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) bytes for whom the algorithm goes to branch A, and goes to branch [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) for the other [![N-M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M) bytes.

For simplicity, we assume that [![C(\mathrm{dict.lookup})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.lookup}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.lookup})), [![C(\mathrm{dict.add})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add})), [![C(\mathrm{dict.membership\_check})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.membership%5C_check}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.membership%5C_check})), [![C(\mathrm{str.concatenate})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.concatenate}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.concatenate})), and [![C(\mathrm{str.copy})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.copy})) are fixed cost that don't vary upon different string sizes. This assumption is invalid/broken for large input, but that kind of case is very rare, so we are good with such hurtless simplification, as long as the strings are of reasonable lengths.

The total cost model of compression process can then be summarized as:

[![
C_{\mathrm{total}} = N * (C(\mathrm{str.concatenate}) + C(\mathrm{dict.membership\_check})) \\
    + M * C(\mathrm{str.copy}) + (N - M) * (C(\mathrm{dict.lookup}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy}))
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.membership%5C_check}))%20%5C%5C%20%20%20%20%20%2B%20M%20*%20C(%5Cmathrm{str.copy})%20%2B%20(N%20-%20M)%20*%20(C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.membership%5C_check}))%20%5C%5C%20%20%20%20%20%2B%20M%20*%20C(%5Cmathrm{str.copy})%20%2B%20(N%20-%20M)%20*%20(C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))

For input data that doesn't have many repeated byte pattern, [![M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) is small compared to [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) (i.e. [![M \ll N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20%5Cll%20N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20%5Cll%20N)). The cost model approximates to:

[![
C_{\mathrm{total}} = N * (C(\mathrm{str.concatenate}) + C(\mathrm{dict.membership\_check}) + C(\mathrm{dict.lookup}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy}))
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.membership%5C_check})%20%2B%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.membership%5C_check})%20%2B%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))

If the underlying data structure implementation of code dict is hash table, then [![C(\mathrm{dict.memebership\_check})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.memebership%5C_check}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.memebership%5C_check})) and [![C(\mathrm{dict.add})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add})) are both [![O(1)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1)) operations. The total cost is [![O(N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)) then.

For input data that has many repeated byte pattern, [![M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) is not negligible compared to [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). The largest possible [![M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) comes from input consisting of single byte pattern. In such case, [![M = O(N - \sqrt{N}) = O(N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20=%20O(N%20-%20%5Csqrt{N})%20=%20O(N))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20=%20O(N%20-%20%5Csqrt{N})%20=%20O(N)) (The detailed deduction process is delegated to the readers). The total cost is still [![O(N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)).

We see that the time complexity of compression algorithm is not affected by the byte repetition pattern. It's always linear time. This nice property holds true as long as the underlying implementation of the code dict scales in sublinear factor.

#### Decompression Algorithm Complexity

Contrary to the compression algorithm, LZW decompression algorithm consumes a stream of codes, decodes them on the fly, and emitted byte data. A string dict is built and maintained along with the process. Similar to the compression process, a running word is maintained during the process. The running word is empty at the beginning.

Deciding on whether the code can be found in the string dict, the algorithm has **two** branches to go.

If the code currently consumed can be found in the string dict, the algorithm goes to branch [![A](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A), where basically four things happen. First, the decoded string coresponding to the current code is lookuped in the string dict. The decoded string is emitted as byte data. Then a new string constructed by appending the first byte of the decoded string to the running word is added to the string dict, ready for future use. The running word is then reset to be the decoded string. The algorithm then goes on to the next iteration.

On the other hand, if the code currently consumed is not present in the string dict, a special situation happens and needs special care. Algorithm goes to branch [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B), where three things happen. A new string constructed by appending the first byte of the running word to the running word is added to the string dict for future use. This new string is emitted as byte data. Then the running word is reset to be the new string. The algorithm then goes on to the next iteration.

The cost model for these two branches is then:

[![
C_A = C(\mathrm{dict.lookup}) + C(\mathrm{str.concatenate}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy})
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))

[![
C_B = C(\mathrm{str.concatenate}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy})
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy}))

Suppose the code stream length is [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). Among the [![N](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) codes consumed by the algorithm, there are [![M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) codes for whom the algorithm goes to branch A, and goes to branch [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) for the other [![N-M](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M) codes.

For simplicity, we assume that [![C(\mathrm{dict.lookup})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.lookup}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.lookup})), [![C(\mathrm{dict.add})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.add})), [![C(\mathrm{dict.membership\_check})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.membership%5C_check}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{dict.membership%5C_check})), [![C(\mathrm{str.concatenate})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.concatenate}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.concatenate})), and [![C(\mathrm{str.copy})](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.copy}))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(%5Cmathrm{str.copy})) are fixed cost that don't vary upon different string sizes. This assumption is invalid/broken for large input, but that kind of case is very rare, so we are good with such hurtless simplification, as long as the strings are of reasonable lengths.

The probability of going to branch [![B](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) is relatively rare, so the major cost comes from branch [![A](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A). The total cost model for the decompression algorithm is then:

[![
C_{\mathrm{total}} = N * (C(\mathrm{dict.membership\_check}) +C(\mathrm{dict.lookup}) + C(\mathrm{str.concatenate}) + C(\mathrm{dict.add}) + C(\mathrm{str.copy}))
](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{dict.membership%5C_check})%20%2BC(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{%5Cmathrm{total}}%20=%20N%20*%20(C(%5Cmathrm{dict.membership%5C_check})%20%2BC(%5Cmathrm{dict.lookup})%20%2B%20C(%5Cmathrm{str.concatenate})%20%2B%20C(%5Cmathrm{dict.add})%20%2B%20C(%5Cmathrm{str.copy})))

It's the same with that of the compression algorithm! The total cost model for the decompression algorithm turns out to be identical to that of the compression algorithm! They are both linear [![O(N)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)). (under the precondition that the underlying implementation of string dict and code dict scales in sublinear factor)

### Performance Analysis

#### Rationale

The algorithmic detail of LZW compression algorithm doesn't have large parameter tuning space or rich variants for twisting. The only performance improvement we can try to make is concentrated on the underlying data structure implementation of the code dict and the string dict. The exported interface consists of `dict.lookup`, `dict.add`, and `dict.membership_check`. Many data structure supports these three operations. When deciding which data structure to use, we want the cost to be as small as possible, because these three operations is invoked in nearly every iteration of the algorithm process. Three candidates are to be examined in detail:

- Balanced Tree

  Balanced tree has the advantages of being easy to implement, while having reasonable average performance bound for many operations. It's too mediocre for our demanding task.

- Hash Table

  Hash table, if implemented carefully, can exhibit excel performance in various frequent operations. The major drawback of hash table data structure is that it's space demanding, and its performance is sensitive to input data pattern. The good news is that we can tune the hashing function and collision resolution scheme to workaround skewed input data.

- Trie

  Trie is a specialized data structure especially good at handling text related data. Its major drawback is that correct and efficient implementation needs some attention and care.

#### Implementation Scheme

We break the trade-off and decide to use hash table to implement the code dict and string dict.

For simplicity, we do not customize our own hashing functions. `std::hash` is used instead. This could serve as a minor optimization candidate for future.

Ideally, the hash table implemented code dict and string dict exhibit [![O(1)](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1))](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1)) time complexity for all `dict.add`, `dict.lookup`, and `dict.membership_check` operations.

## Hacks and Tricks

### Stream Style IO

Instead of loading the whole file content into memory, which could lead to large memory footprint and hence resource pressure, our code adopts a more flexible IO method. We load input data in stream style. We read in data when needed and no too much more. This ensures that our code scales well with large input data size. Also our code is more memory space efficient.

### Reserve Capacity to Reduce Resizing Cost

A little trick we use here is to reserve sufficient space for the code dict and string dict at initialization. This effectively reduce the cost of resizing hash table, which is expensive, to say the least.

### Cached Incremental Hash State to Reduce Recomputation Cost

Deprecated.

### Immutable Data Structure to Reduce Space Cost

Deprecated.

## License

[WTFPL 2.0](./LICENSE)

[![WTFPL](http://www.wtfpl.net/wp-content/uploads/2012/12/wtfpl-badge-1.png)](http://www.wtfpl.net/)


## TODO

<details open>
<summary>details</summary>

## TODO

- Performance improvement. Profile to find performance hotspots. Use more advanced data structure to replace the naive hash table implementation.
- Investigate hash table statistics (e.g., load factor, hit/miss rate, collision count)
- Use more efficient hash algorithm for string data structure
- Use more efficient hash table design algorithm. (cf. TAOCP Vol.3)
- Consider use `.hpp` instead of `.h`
- Add `const` modifier to member function that doesn't change member variables.
- Choose semantic exception type
- Use custom exceptions instead of lazily using `runtime_error` for every error places.
- Inline member function
- Add `const` modifier to parameter
- Add print to `cerr` before throwing exception
- Use Pass-by-reference for some parameters
- Use command `nm` to check object files' symbols.
- Add imported name as comment beside include directive
- Remove unused includes.
- Add unit test for all interface. Calculate test coverage with `gcov`.
- Build shared/dynamic library
- Add `inline` keyword to some short functions
- Acquire empirical statistics of hash table collision condition.
- Use optimization flag to compile before making performance benchmark.
- Remove dead codes, outdated comments. Cleanup.
- Consider separate template declaration and definition, refer to https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
- Consider use bitmap to implement Bitarray, for improving performance.
- Use pytest-subtesthack.
- Check for memory leak. (e.g. unmatched new-delete, malloc-free) Or use automatic tool, like valgrind.
- Add \[SCOPE\] before every entry in TODO.md
- Project idea: script to auto-generate Makefile
  - Turn out that `g++ -M` can do the job.
- Restructure project layout. `#include "lzw/utils.h"` instead of `#include "utils.h"`
- Use `std::copy`, `std::hash`, `std::move`, `std::memcpy`, `std::map`, `std::swap`.
- Implement variants of LZW algorithm. E.g., code size, code table whether to reuse. Encode algorithm parameters to lzwfile and let decode logic automatically inspect it.
- The canonical copy-assignment operator is expected to perform no action on self-assignment. Ref: https://en.cppreference.com/w/cpp/language/operators
- Other overloaded operator also some of them expect no action performed on self-assignment. Ref: https://en.cppreference.com/w/cpp/language/operators
- Use immutable data structure to improve space efficiency.
- Use VSCode C/C++ extension's rename functionality to rename `Code` to `int` and `Bytes` to `string`.
- Try to use and master VSCode C/C++ extension's debug functionality.
- Compare speed of LZW.py and liblzw
- Wrap liblzw into CPython extension. Remove subprocess call overhead when testing using Python Hypothesis framework.
  - Consider using Cython to fast prototype CPython extension.
- Update CMakeLists.txt
- Setup unit-test workflow. Include CI badge, about test suit passing status, and test coverage percentage.
- Try C++ test frameworks.
- Add GitHub badge about LOC (lines of code), or SLOC.
- Add GitHub badge about "this repo uses semantic versioning".
- State least supported C++ versions in README.
- Consider deploying [auto-changelog-hook](https://github.com/MartinSeeler/auto-changelog-hook)
- Consider align README.md big title to the center of the page.
- Add instructions to build dynamic library / shared library in README
- In pre-commit hook script, use black and isort as Python moduel instead of invoke as subprocess, to reduce subprocess call overhead.


## Archive

- Use `std::bitset` to replace self-made bitmap data structure.
- Reimplement `Bytes` and `Bitarray` data strcutre. Use more efficient implementation.
- Use C++11 move semantic to improve performance in vector resize process.
- Put short inline-able member functions to header
- clangformat configuration about `using namespace` statement
- Add CUHK academic honesty header


## TIL

- Remeber to use flag `-static-libstdc++` when compiling C++ in MinGW Windows environment.
- gcc's -I option "add the directory dir to the list of directories to be searched for header files. Directories named by -I are searched before the standard system include directories.", refer to: https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Preprocessor-Options.html#Preprocessor-Options

</details>

## CHANGELOG

<details open>
<summary>details</summary>

## Changelog

- Multiple inclusion guard
- Template class can't separate declaration and definition into two files.
- Don't use `using namespace std;` in header files
- Don't use `malloc` in C++. It can't correctly handle class initialization.
- [str_dict] Add `str_cache` to reduce string membership check from `O(n)` to `O(1)`
- Use `.tpp` as extension for templates
- Use `std::string` and specialized `std::hash<std::string>` function object to replace wrapper type `Bytes`.
- Use int and specialized `std::hash<int>` function object ot replace wrapper type `Code`.
- Use `std::unordered_map` to replace self-made `dict` data structure.
- Use `std::set` to replace self-made `set` data structure.
- Use `std::hash`
- Outdo lzw_example_win.exe
- Out-of-source build
- Build static library: liblzw.a
- Improve CLI interface to be more user-friendly and intuitive.
- Write README. Elaborate on algorithmic details. Elaborate on analysis of computational complexity.
- Add git tag "submission milestone"
- Customize commit hook: process README.raw.md to README.md with all latex equation transformed to images.
- Add Travis CI pipeline. Automatically compile and test.
- Create script to replace LaTeX math text with image url. For render in GitHub.
- Release prebuild binary (static library, cli executable) and version-tagged source tarball.
- Push to GitHub upstream.
- Remove sensitive information. The first commit.
- Add unit test for bitmap data structure.
- Add GitHub badge about release version.
- Utilize Makefile syntax (order-only prerequisites) to specify that some directory need to pre-exist
- In pre-commit hook script, re-generate README TOC and insert.

</details>
