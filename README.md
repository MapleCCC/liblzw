# LZW Compressor

[![License](https://img.shields.io/github/license/MapleCCC/liblzw?color=00BFFF)](http://www.wtfpl.net/)
[![travisci](https://www.travis-ci.com/MapleCCC/liblzw.svg?branch=master)](https://travis-ci.com/MapleCCC/liblzw)

## Introduction

LZW is an archive format that utilize power of LZW compression algorithm. LZW compression algorithm is a dictionary-based loseless algorithm. It's an old algorithm suitable for beginner to practice.

Internal algorithm processes byte data. So it's applicable to any file types, besides text file. Although it may not be able to achieve substantial compression rate for some file types that are already compressed efficiently, such as PDF files and MP4 files. It treats data as byte stream, unaware of the text-level pattern, which makes it less compression-efficient compared to other more advanced compression algorithms.

LZW compression algorithm is dynamic. It doesn't collect data statistics before hand. Instead, it learns the data pattern while conducting the compression, building a code table on the fly. The compression ratio approaches maximum after enough time. The algorithmic complexity is strictly linear to the size of the text. [A more in-depth algorithmic analysis can be found in the following sections.](#algorithmic-analysis)

An alternative implementation that utilizes more efficient self-made customized `bitmap`, `dict` and `set` to replace C++ builtin general-purpose `std::bitset`, `std::unordered_map` and `std::set` can be found in the branch [`assignment`](https://github.com/MapleCCC/liblzw/tree/assignment). Future enhancement includes customized `hash` functions to replace builtin general-purpose `std::hash`.

## Installation

Pre-built binaries (Windows only) are available on the [Releases](https://github.com/MapleCCC/liblzw/releases/latest) page.

## Build

```bash
# if g++.exe is supported:
$ make
# Compiled executable is in build/lzw.exe

# if speed is desirable:
$ make fast

# if cl.exe is supported:
$ mkdir build && cl /Fe"build/lzw.exe" lzw.cpp
# Compiled executable is in build/lzw.exe

# if speed is desirable:
# compile in release mode
$ mkdir build && cl /D "NDEBUG" /O2 /Fe"build/lzw.exe" lzw.cpp
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

```bash
$ git clone https://github.com/MapleCCC/liblzw.git
$ cp scripts/pre-commit.py .git/hooks/pre-commit
```

The pre-commit hook script basically does two things:

1. Format staged C/C++ code

2. Transform `LaTeX` math equation in `README.raw.md` to image url in `README.md`

Besides relying on the pre-commit hook script, you can manually format code and transform math equations in `README.md`.

```bash
$ make reformat
$ make transform-eqn
```

The advantages of pre-commit hook script, compared to manual triggering scripts, is that it's convenient and un-disruptive, as it only introduces changes to staged files, not all files in the repo.

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

Before talking about performance, we first conduct some complexity analysis first. This help us understand the internal structure of the problem, and which and how various parts of the task contribute to the performance hotspots.

#### Compression Algorithm Complexity

LZW algorithm treats data as binary stream, regardless of its text encoding. During the algorithm process, a running word is maintained. Before algorithm starts, the running word is empty. Every new byte consumed by the algorithm would be appended to the running word. The running word would be reset to some value under some circumstances.

For each byte consumed by the algorithm, deciding upon whether the running word is in the code dict, the algorithm has **two** branches to go.

If the new running word formed by appending the new byte at the end is in the code dict, the algorithm proceeds to ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A) branch, which basically do nothing, and continue to next iteration. On the other hand, if the new running word constituted by the new byte is not in the code dict, the algorithm goes to ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) branch.

In the ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) branch, three things happen. First, the code in the code dict corresponding to the old running word (before the new byte is appended) is emitted. Then the new running word is entered into the code dict as a new string, and assigned a new code, ready to use in the future. Lastly, the running word is reset to contain only the new byte. The algorithm then proceeds to the next iteration.

The cost model for these two branches are respectively:

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(\mathrm{str.copy}))

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(\mathrm{dict.lookup})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy}))

Suppose the source text byte length is ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). Among the ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) bytes consumed by the algorithm, there are ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) bytes for whom the algorithm goes to branch A, and goes to branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) for the other ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M) bytes.

For simplicity, we assume that ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.lookup})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.add})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.membership-check})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{str.concatenate})), and ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{str.copy})) are fixed cost that don't vary upon different string sizes. This assumption is invalid/broken for large input, but that kind of case is very rare, so we are good with such hurtless simplification, as long as the strings are of reasonable lengths.

The total cost model of compression process can then be summarized as:

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{\mathrm{total}}%20=%20N%20*%20(C(\mathrm{str.concatenate})%20%2B%20C(\mathrm{dict.membership-check}))%20\\%20%20%20%20%20%2B%20M%20*%20C(\mathrm{str.copy})%20%2B%20(N%20-%20M)%20*%20(C(\mathrm{dict.lookup})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy})))

For input data that doesn't have many repeated byte pattern, ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) is small compared to ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) (i.e. ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20\ll%20N)). The cost model approximates to:

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{\mathrm{total}}%20=%20N%20*%20(C(\mathrm{str.concatenate})%20%2B%20C(\mathrm{dict.membership-check})%20%2B%20C(\mathrm{dict.lookup})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy})))

If the underlying data structure implementation of code dict is hash table, then ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.memebership\_check})) and ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.add})) are both ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1)) operations. The total cost is ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)) then.

For input data that has many repeated byte pattern, ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) is not negligible compared to ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). The largest possible ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) comes from input consisting of single byte pattern. In such case, ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M%20=%20O(N%20-%20\sqrt{N})%20=%20O(N)) (The detailed deduction process is delegated to the readers). The total cost is still ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)).

We see that the time complexity of compression algorithm is not affected by the byte repetition pattern. It's always linear time. This nice property holds true as long as the underlying implementation of the code dict scales in sublinear factor.

#### Decompression Algorithm Complexity

Contrary to the compression algorithm, LZW decompression algorithm consumes a stream of codes, decodes them on the fly, and emitted byte data. A string dict is built and maintained along with the process. Similar to the compression process, a running word is maintained during the process. The running word is empty at the beginning.

Deciding on whether the code can be found in the string dict, the algorithm has **two** branches to go.

If the code currently consumed can be found in the string dict, the algorithm goes to branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A), where basically four things happen. First, the decoded string coresponding to the current code is lookuped in the string dict. The decoded string is emitted as byte data. Then a new string constructed by appending the first byte of the decoded string to the running word is added to the string dict, ready for future use. The running word is then reset to be the decoded string. The algorithm then goes on to the next iteration.

On the other hand, if the code currently consumed is not present in the string dict, a special situation happens and needs special care. Algorithm goes to branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B), where three things happen. A new string constructed by appending the first byte of the running word to the running word is added to the string dict for future use. This new string is emitted as byte data. Then the running word is reset to be the new string. The algorithm then goes on to the next iteration.

The cost model for these two branches is then:

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_A%20=%20C(\mathrm{dict.lookup})%20%2B%20C(\mathrm{str.concatenate})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy}))

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_B%20=%20C(\mathrm{str.concatenate})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy}))

Suppose the code stream length is ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N). Among the ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N) codes consumed by the algorithm, there are ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;M) codes for whom the algorithm goes to branch A, and goes to branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) for the other ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;N-M) codes.

For simplicity, we assume that ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.lookup})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.add})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{dict.membership-check})), ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{str.concatenate})), and ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;C(\mathrm{str.copy})) are fixed cost that don't vary upon different string sizes. This assumption is invalid/broken for large input, but that kind of case is very rare, so we are good with such hurtless simplification, as long as the strings are of reasonable lengths.

The probability of going to branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;B) is relatively rare, so the major cost comes from branch ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;A). The total cost model for the decompression algorithm is then:

![](https://latex.codecogs.com/svg.latex?\fn_cm&space;\small&space;C_{\mathrm{total}}%20=%20N%20*%20(C(\mathrm{dict.membership-check})%20%2BC(\mathrm{dict.lookup})%20%2B%20C(\mathrm{str.concatenate})%20%2B%20C(\mathrm{dict.add})%20%2B%20C(\mathrm{str.copy})))

It's the same with that of the compression algorithm! The total cost model for the decompression algorithm turns out to be identical to that of the compression algorithm! They are both linear ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(N)). (under the precondition that the underlying implementation of string dict and code dict scales in sublinear factor)

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

Ideally, the hash table implemented code dict and string dict exhibit ![](https://latex.codecogs.com/svg.latex?\inline&space;\fn_cm&space;\small&space;O(1)) time complexity for all `dict.add`, `dict.lookup`, and `dict.membership_check` operations.

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
