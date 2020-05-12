# Assignment 3 - LZW Compression

## Compile

```bash
# if g++.exe is supported:
g++ --std=c++11 -static-libstdc++ -Wall -Wextra -g lzw.cpp -o lzw

# if speed is desirable:
g++ --std=c++11 -static-libstdc++ -Ofast lzw.cpp -o lzw

# if cl.exe is supported:
cl lzw.cpp
```

## Usage

```bash
# Compression
./lzw -c <lzw filename> <a list of files>

# Decompression
./lzw -d <lzw filename>
```

## Test

```bash
make unit-test

# Python3.6 is needed for integrate test
make integrate-test
```

## Performance
