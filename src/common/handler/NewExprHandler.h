//
// Created by dutow on 10/4/15.
//

#ifndef TYPEGRIND_CLANG_NEWEXPRHANDLER_H
#define TYPEGRIND_CLANG_NEWEXPRHANDLER_H

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

namespace typegrind {
    class NewExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        NewExprHandler(clang::Rewriter*& rewriter);
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

        clang::StringRef getID() const override;

    private:
        clang::Rewriter*& mRewriter;
    };
}



#endif //TYPEGRIND_CLANG_NEWEXPRHANDLER_H
