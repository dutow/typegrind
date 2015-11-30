
#include "DeleteExprHandler.h"

namespace typegrind {

    DeleteExprHandler::DeleteExprHandler(clang::Rewriter *&rewriter)
            : mRewriter(rewriter) {
    }

    void DeleteExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result) {
        const clang::CXXDeleteExpr* deleteExpr = result.Nodes.getNodeAs<clang::CXXDeleteExpr>("deleteStmt");
        if (nullptr == deleteExpr)
        {
            llvm::errs() << "Couldn't convert MatcherResult to CXXDeleteExpr!\n";
            return;
        }

        std::string macroStart = "TYPEGRIND_LOG_DEALLOC";

        if (deleteExpr->isArrayForm()) {
            macroStart += "_ARRAY";
        }
        macroStart += "(";

        // 1st parameter: pointer
        {
            llvm::raw_string_ostream os(macroStart);
            // TODO: this assumes that the size expression is const, which isn't always true! => extract expression to a variable
            deleteExpr->getArgument()->printPretty(os, nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
            os.flush();
        }
        macroStart += ", ";

        // 2nd parameter: source location
        auto& sm = result.Context->getSourceManager();
        auto ploc = sm.getPresumedLoc(deleteExpr->getLocEnd());
        macroStart += "\"";
        macroStart += ploc.getFilename();
        macroStart += ":";
        macroStart += std::to_string(ploc.getLine());
        macroStart += "\", ";

        clang::SourceLocation startLoc = deleteExpr->getLocStart();
        mRewriter->InsertText(startLoc, macroStart);

        // 3rd parameter: the expression itself
        // end added function call
        std::string macroEnd = ")";

        clang::SourceLocation endLoc = deleteExpr->getLocEnd();
        mRewriter->InsertTextAfterToken(endLoc, macroEnd);
    }

    clang::StringRef DeleteExprHandler::getID() const {
        return "typegrind";
    }
}
