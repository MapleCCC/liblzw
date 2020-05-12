# Assignment 3 - LZW Compression

## Compile

```bash
# if g++.exe is supported:
make
# Compiled executable is in build/lzw.exe

# if speed is desirable:
make fast

# if cl.exe is supported:
cl lzw.cpp
```

## Usage

```bash
# Compression
lzw -c <lzw filename> <a list of files>

# Decompression
lzw -d <lzw filename>
```

## Test

```bash
# Python3.6+, Pytest and Hypothesis are needed for integrate test
pip install pytest, hypothesis
make integrate-test
```

## Performance
