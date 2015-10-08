Building Typegrind
===

Based on: [http://clang.llvm.org/docs/LibASTMatchersTutorial.html](http://clang.llvm.org/docs/LibASTMatchersTutorial.html)


Required software
---

 * git
 * a working C++ build environment


Step 1: Obtaining Clang
---

```bash
mkdir ~/clang-llvm && cd ~/clang-llvm
git clone http://llvm.org/git/llvm.git
cd llvm/tools
git clone http://llvm.org/git/clang.git
cd clang/tools
git clone http://llvm.org/git/clang-tools-extra.git extra
```


Step 2: Obtain Typegrind
---

```bash
cd extra
git clone https://github.com/dutow/typegrind.git
``` 


Step 3: Add Typegrind to the build process
---

```
echo "add_subdirectory(typegrind)" >> CMakeFiles.txt
```


Step 4: Let's build everything!
---

```bash
cd ~/clang-llvm
mkdir build && cd build
cmake ../lvm -D ... # add any parameters you want to
cmake --build .
```


