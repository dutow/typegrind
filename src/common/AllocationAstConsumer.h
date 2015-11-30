
#ifndef TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
#define TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H

#include <clang/Frontend/ASTUnit.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "common/handler/NewExprHandler.h"
#include "common/handler/OpNewExprHandler.h"
#include "common/handler/DeleteExprHandler.h"

namespace typegrind {
    class AllocationASTConsumer : public clang::ASTConsumer {
    public:
        AllocationASTConsumer(clang::Rewriter*& rewriter);

        void HandleTranslationUnit(clang::ASTContext& context) override;

    private:
        clang::ast_matchers::MatchFinder mMatcher;

        clang::Rewriter*& mRewriter;

        NewExprHandler mNewExprHandler;
        OpNewExprHandler mOpNewExprHandler;
        DeleteExprHandler mDeleteExprHandler;
    };
}

#endif //TYPEGRIND_CLANG_ALLOCATIONASTCONSUMER_H
