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

    AllocationASTConsumer::AllocationASTConsumer(clang::Rewriter& rewriter)
            : mRewriter(rewriter)
            , mNewExprHandler(mRewriter)
    {
        mMatcher.addMatcher(ast_matchers::newExpr().bind("newStmt"), &mNewExprHandler);
    }

    void AllocationASTConsumer::HandleTranslationUnit(ASTContext& context)
    {
        mMatcher.matchAST(context);
    }
}

