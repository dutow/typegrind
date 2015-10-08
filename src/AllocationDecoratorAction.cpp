
#include "AllocationDecoratorAction.h"

namespace typegrind {

    AllocationDecoratorAction::AllocationDecoratorAction(clang::Rewriter &rewriter) : mRewriter(rewriter) { }

    bool AllocationDecoratorAction::isCpp(clang::CompilerInstance const& Compiler) {
        clang::LangOptions const Opts = Compiler.getLangOpts();
        return Opts.CPlusPlus;
    }

    // ..:: Entry point for plugins ::..
    std::unique_ptr<clang::ASTConsumer> AllocationDecoratorAction::newASTConsumer() {
        return std::unique_ptr<clang::ASTConsumer>(new AllocationASTConsumer(mRewriter));
    }

}
