## TODO

- Performance improvement. Profile to find performance hotspots. Use more advanced data structure to replace the naive hash table implementation.
- Investigate hash table statistics (e.g., load factor, hit/miss rate, collision count)
- Use more efficient hash algorithm for string data structure
- Use more efficient hash table design algorithm. (cf. TAOCP Vol.3)
- Add CUHK academic honesty header
- Release prebuild binary (static library, cli executable) and version-tagged source tarball.
- Consider use .hpp instead of .h
- Add const modifier to member function that doesn't change member variables.
- Choose semantic exception type
- Use custom exceptions instead of lazily using runtime_error for every error places.
- Inline member function
- Add const modifier to parameter
- Add print to cerr before throwing exception
- Use Pass-by-reference for some parameters
- Use command `nm` to check object files' symbols.
- Add imported name as comment beside include directive
- Add unit test for bitmap data structure.
- Remove unused includes.
- Add unit test for all interface. Calculate test coverage with gcov.
- Build shared/dynamic library
- Add inline keyword to some short functions
- Acquire empirical statistics of hash table collision condition.
- Use optimization flag to compile before making performance benchmark.
- Remove dead codes, outdated comments. Cleanup.
- Consider separate template declaration and definition, refer to https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
- Consider use bitmap to implement Bitarray, for improving performance.
- Use pytest-subtesthack.
- Check for memory leak. (e.g. unmatched new-delete, malloc-free) Or use automatic tool, like valgrind.
- Add \[SCOPE\] before every entry in TODO.md
- Project idea: script to auto-generate Makefile
- Push to GitHub upstream.
- Restructure project layout. #include "lzw/utils.h" instead of #include "utils.h"
- Use std::copy, std::hash, std::move, std::memcpy, std::map, std::swap.
- Implement variants of LZW algorithm. E.g., code size, code table whether to reuse. Encode algorithm parameters to lzwfile and let decode logic automatically inspect it.
- The canonical copy-assignment operator is expected to perform no action on self-assignment. Ref: https://en.cppreference.com/w/cpp/language/operators
- Other overloaded operator also some of them expect no action performed on self-assignment. Ref: https://en.cppreference.com/w/cpp/language/operators
- Write README. Elaborate on algorithmic details.
- Use immutable data structure to improve space efficiency.
- Use VSCode C/C++ extension's rename functionality to rename Code to int and Bytes to string.
- Try to use and master VSCode C/C++ extension's debug functionality.


## Done

- Multiple inclusion guard
- Template class can't separate declaration and definition into two files.
- Don't use "using namespace std;" in header files
- Don't use malloc in C++. It can't correctly handle class initialization.
- [str_dict] Add str_cache to reduce string membership check from O(n) to O(1)
- Use .tpp as extension for templates
- Use std::string and specialized std::hash<std::string> function object to replace wrapper type Bytes.
- Use int and specialized std::hash<int> function object ot replace wrapper type Code.
- Use std::unordered_map to replace self-made dict data structure.
- Use std::set to replace self-made set data structure.
- Use std::hash
- Outdo lzw_example_win.exe
- Out-of-source build
- Build static library: liblzw.a


## Archive

- Use std::bitset to replace self-made bitmap data structure.
- Reimplement Bytes and Bitarray data strcutre. Use more efficient implementation.
- Use C++11 move semantic to improve performance in vector resize process.
- Put short inline-able member functions to header


## TIL

- Remeber to use flag -static-libstdc++ when compiling C++ in MinGW Windows environment.
- gcc's -I option "add the directory dir to the list of directories to be searched for header files. Directories named by -I are searched before the standard system include directories.", refer to: https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Preprocessor-Options.html#Preprocessor-Options
