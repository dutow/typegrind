
file( GLOB_RECURSE COMMON_SOURCES ../common/*.cpp )

add_clang_executable(clang-typegrind 
  ${COMMON_SOURCES}
  CompilerLikeOptionsParser.cpp
  main.cpp
)

target_link_libraries(clang-typegrind
  clangBasic
  clangTooling
  clangDriver
  clangFrontend
  clangAST
  clangASTMatchers
  clangRewrite
  )

install(TARGETS clang-typegrind RUNTIME DESTINATION bin)
