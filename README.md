Typegrind
===

Typegrind is a type preserving heap profiler for C++ - it collects memory allocation information with type information.


Components
---

Typegrind consists of two main components:

 * Instrumentator, which is a source-to-source compiler, decorating the C++ source with code required for the logger.
 * Loggers, which do something useful with the information provided by the instrumented code (e.g. write it to a logfile).

Both components can be used separatedly: the instrumentator with a custom logger, or the loggers with manually or otherwise decorated code. This is accomplished with a simple API between them, consisting of a few macros. For details, see `doc/logging_api.md`.


Building
---

Typegrind can be build as a part of the clang build process, for details see `doc/building.md`


Usage
---

Typegrind is a Clang Tool, and uses the same parameters as other, builtin tools like `clang-check` or `clang` itself.

For details, see `doc/usage.md`.

TODO TODO TODO


Example
---

The original source code:

```cpp
#include <typegrind/log_to_cout.hpp>
#include <iostream>

int main() {
  int* a = new int(3);
  std::cout << *a << std::endl;
  delete a;
  return 0;
}
```

Is transformed to:

```cpp
#include <typegrind/log_to_cout.hpp>
#include <iostream>

int main() {
  int* a = TYPEGRIND_LOG_ALLOC("int*", "example.cpp:6", new int(3), sizeof(int));
  std::cout << *a << std::endl;
  TYPEGRIND_LOG_DEALLOC(a, delete a);
  return 0;
}
```

Which contains two macros defined in `typegrind/log_to_cout.hpp`, and expands to:

```cpp
#include <typegrind/log_to_cout.hpp>
#include <iostream>

int main() {
  int* a = typegrind::log_cout::alloc("int*", "example.cpp:6", (new int(3)), sizeof(int));
  std::cout << *a << std::endl;
  typegrind::log_cout::dealloc(a); (delete a);
  return 0;
}
```


Known limitations
---

 * Exotic macros related to object allocation will cause compilation errors. For example:
   ```cpp
     #define DECL_AN_INT(name) int* name = new int(0);
   ```
 * Delete loggers might call the pointer expression twice - once for logging, once for deleting it. It has to be extracted into a variable before use.
 * Include directives / linker settings aren't modified by typegrind. The projects using it should include the desired logger, and link to it's library if it has one.


Future work
---

 * Solve the above limitations
 * Implement the planned macros in the API (see API docs for details)
 * Improve the usage process (call clang with an in-memory VFS automatically)
 * Create production ready standard loggers
 * Create a user friendly logger frontend


Documentation
---

The documentation can be found under the `doc` folder in the source, written in markdown, browsable on GitHub.


License
---

Typekit is published under the 
[MIT License](https://opensource.org/licenses/MIT)