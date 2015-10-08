set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangBasic clangTooling clangDriver clangFrontend clangAST clangASTMatchers clangRewrite)

ADD_SUBDIRECTORY(src)
