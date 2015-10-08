//
// Created by dutow on 10/7/15.
//

#ifndef TPHP_CLANG_OPNEWEXPRHANDLER_H
#define TPHP_CLANG_OPNEWEXPRHANDLER_H

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

namespace typegrind {
    class OpNewExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        OpNewExprHandler(clang::Rewriter*& rewriter);
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

        clang::StringRef getID() const override;

    private:
        clang::Rewriter*& mRewriter;
    };
}

#endif //TPHP_CLANG_OPNEWEXPRHANDLER_H
