
file( GLOB_RECURSE COMMON_SOURCES ../common/*.cpp )

add_llvm_loadable_module(typegrind-plugin
  ${COMMON_SOURCES}
  WrapperAction.cpp
)

target_link_libraries(typegrind-plugin
  clangBasic
  clangTooling
  clangDriver
  clangFrontend
  clangAST
  clangASTMatchers
  clangRewrite
  )
