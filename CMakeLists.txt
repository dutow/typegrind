set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangBasic clangTooling clangDriver clangFrontend clangAST clangASTMatchers clangRewrite)

INCLUDE_DIRECTORIES(src)
ADD_SUBDIRECTORY(src)

