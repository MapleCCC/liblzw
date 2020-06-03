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


### Archive

- Use `std::bitset` to replace self-made bitmap data structure.
- Reimplement `Bytes` and `Bitarray` data strcutre. Use more efficient implementation.
- Use C++11 move semantic to improve performance in vector resize process.
- Put short inline-able member functions to header
- clangformat configuration about `using namespace` statement
- Add CUHK academic honesty header


### TIL

- Remeber to use flag `-static-libstdc++` when compiling C++ in MinGW Windows environment.
- gcc's -I option "add the directory dir to the list of directories to be searched for header files. Directories named by -I are searched before the standard system include directories.", refer to: https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Preprocessor-Options.html#Preprocessor-Options
