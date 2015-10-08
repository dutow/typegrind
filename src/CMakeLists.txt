
add_clang_executable(clang-typegrind 
  AllocationAstConsumer.cpp
  AllocationDecoratorAction.cpp
  NewExprHandler.cpp
  OpNewExprHandler.cpp
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
