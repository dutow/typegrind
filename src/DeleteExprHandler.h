#ifndef TYPEGRIND_DELETEEXPRHANDLER_H
#define TYPEGRIND_DELETEEXPRHANDLER_H

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>

namespace typegrind {
    class DeleteExprHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        DeleteExprHandler(clang::Rewriter*& rewriter);
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

        clang::StringRef getID() const override;

    private:
        clang::Rewriter*& mRewriter;
    };
}

#endif //TYPEGRIND_DELETEEXPRHANDLER_H
