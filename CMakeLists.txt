set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(typegrind
  main.cpp
  )

target_link_libraries(typegrind
  clangTooling
  clangBasic
  clangASTMatchers
  )
