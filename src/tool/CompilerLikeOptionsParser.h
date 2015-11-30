
#ifndef LLVM_COMPILERLIKEOPTIONSPARSER_H
#define LLVM_COMPILERLIKEOPTIONSPARSER_H

#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/Support/CommandLine.h"

class CompilerLikeOptionsParser {
public:

    CompilerLikeOptionsParser(int &argc, const char **argv,
                        llvm::cl::OptionCategory &Category,
                        const char *Overview = nullptr);

    /// Returns a reference to the loaded compilations database.
    clang::tooling::CompilationDatabase &getCompilations() {
        return *Compilations;
    }

    /// Returns a list of source file paths to process.
    std::vector<std::string> getSourcePathList() {
        return SourcePathList;
    }

private:
    std::unique_ptr<clang::tooling::CompilationDatabase> Compilations;
    std::vector<std::string> SourcePathList;
};

#endif //LLVM_COMPILERLIKEOPTIONSPARSER_H
