
#ifndef TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
#define TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H

#include <memory>

#include <llvm/Support/CommandLine.h>

#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Frontend/CompilerInstance.h>

#include "AllocationAstConsumer.h"

namespace typegrind {

    class AllocationDecoratorAction {
    public:
        AllocationDecoratorAction(clang::Rewriter& rewriter);

        AllocationDecoratorAction & operator=(AllocationDecoratorAction const &) = delete;

        std::unique_ptr<clang::ASTConsumer> newASTConsumer();
    private:

        static bool isCpp(clang::CompilerInstance const& Compiler);

        clang::Rewriter& mRewriter;
    };

}

#endif //TYPEGRIND_CLANG_ALLOCATIONDECORATORACTION_H
