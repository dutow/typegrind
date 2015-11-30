//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "AllocationAstConsumer.h"

#include <clang/ASTMatchers/ASTMatchers.h>


using namespace clang;

namespace typegrind{

    AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter*& rewriter)
            : mRewriter(rewriter)
            , mNewExprHandler(mRewriter)
            , mOpNewExprHandler(mRewriter)
            , mDeleteExprHandler(mRewriter)
    {
        using namespace ast_matchers;
        mMatcher.addMatcher(newExpr().bind("newStmt"), &mNewExprHandler);
        mMatcher.addMatcher(
                callExpr(callee(functionDecl(hasName("operator new"))), hasAncestor(staticCastExpr().bind("castExpr"))).bind("newStmt") ,
                &mOpNewExprHandler
        );
        mMatcher.addMatcher(
                callExpr(callee(functionDecl(hasName("operator new"))), hasAncestor(reinterpretCastExpr().bind("castExpr"))).bind("newStmt"),
                &mOpNewExprHandler
        );
        mMatcher.addMatcher(deleteExpr().bind("deleteStmt"), &mDeleteExprHandler);
    }

    void AllocationASTConsumer::HandleTranslationUnit(ASTContext& context)
    {
        if (mRewriter == nullptr) {
            mRewriter = new clang::Rewriter(context.getSourceManager(), context.getLangOpts());
        }
        mMatcher.matchAST(context);
    }
}

