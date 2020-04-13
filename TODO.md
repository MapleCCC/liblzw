## TODO

- Performance improvement. Profile to find performance hotspots. Use more advanced data structure to replace the naive hash table implementation.
- Investigate hash table statistics (e.g., load factor, hit/miss rate)
- Use more efficient hash algorithm
- Out-of-source build
- Add CUHK academic honesty header
- Release prebuild binary and version-tagged source tarball.
- Consider use .hpp instead of .h
- Add const modifier to member function that doesn't change member variables.
- Choose semantic exception type
- Inline member function
- Add const modifier to parameter
- Add print to cerr before throwing exception
- Use Pass-by-reference for some parameters
- Use command `nm` to check object files' symbols.
- Add imported name as comment beside include directive
- Add unit test for bitmap data structure.
- Remove unused includes.

## Done

- Multiple inclusion guard
- Template class can't separate declaration and definition into two files.
- Don't use "using namespace std;" in header files
- Don't use malloc in C++. It can't correctly handle class initialization.
