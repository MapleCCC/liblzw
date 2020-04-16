## TODO

- Performance improvement. Profile to find performance hotspots. Use more advanced data structure to replace the naive hash table implementation.
- Investigate hash table statistics (e.g., load factor, hit/miss rate, collision count)
- Use more efficient hash algorithm for string data structure
- Use more efficient hash table design algorithm.
- Out-of-source build
- Add CUHK academic honesty header
- Release prebuild binary (static library, cli executable) and version-tagged source tarball.
- Consider use .hpp instead of .h
- Add const modifier to member function that doesn't change member variables.
- Choose semantic exception type
- Custom exceptions
- Inline member function
- Add const modifier to parameter
- Add print to cerr before throwing exception
- Use Pass-by-reference for some parameters
- Use command `nm` to check object files' symbols.
- Add imported name as comment beside include directive
- Add unit test for bitmap data structure.
- Remove unused includes.
- Add unit test for all interface. Calculate test coverage with gcov.
- Build static library: liblzw.a
- Build shared library
- Add inline keyword to some short functions
- Acquire empirical statistics of hash table collision condition.
- Use optimization flag to compile before making performance benchmark.
- Remove dead codes, outdated comments. Cleanup.
- Consider separate template declaration and definition, refer to https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
- Consider use bitmap to implement Bitarray, for improving performance.
- Put short inline-able member functions to header
- Use pytest-subtesthack.
- Use std::hash
- gcc's -I option "add the directory dir to the list of directories to be searched for header files. Directories named by -I are searched before the standard system include directories.", refer to: https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Preprocessor-Options.html#Preprocessor-Options
- Check for memory leak. (e.g. unmatched new-delete, malloc-free)
- Outdo lzw_example_win.exe
- Add [SCOPE] before every entry in TODO.md

## Done

- Multiple inclusion guard
- Template class can't separate declaration and definition into two files.
- Don't use "using namespace std;" in header files
- Don't use malloc in C++. It can't correctly handle class initialization.
- [str_dict] Add str_cache to reduce string membership check from O(n) to O(1)
- Use .tpp as extension for templates
