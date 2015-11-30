//
// Created by dutow on 10/7/15.
//

#include "OpNewExprHandler.h"

namespace typegrind {
    OpNewExprHandler::OpNewExprHandler(clang::Rewriter*& rewriter)
            : mRewriter(rewriter)
    {
    }

    void OpNewExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
    {
        const clang::ExplicitCastExpr* castExpr = result.Nodes.getNodeAs<clang::ExplicitCastExpr>("castExpr");
        const clang::CallExpr* newExpr = result.Nodes.getNodeAs<clang::CallExpr>("newStmt");
        if (nullptr == castExpr)
        {
            llvm::errs() << "Couldn't convert MatcherResult to ExplicitCastExpr!\n";
            return;
        }
        if (nullptr == newExpr)
        {
            llvm::errs() << "Couldn't convert MatcherResult to CallExpr!\n";
            return;
        }
        // Skipping substituted template types
        auto srcType = castExpr->getTypeInfoAsWritten()->getTypeLoc();
        auto ptr = srcType.getType().getTypePtr()->getAs<clang::SubstTemplateTypeParmType>();
        if (ptr) {
            // This is an automatic template specialization.
            // TODO: add additional checks that this condition is good! TBH I have no idea why it works, I should read more CLANG apidocs :)
            // I mean:
            // * partial template specialization
            // * why isn't a specialized template a substitution? (or maybe it's in the standard?)
            return;
        }

        std::string macroStart = "TYPEGRIND_LOG_OP_NEW";
        macroStart += "(";

        // 1st paramter: name of the type.
        // Currently it is the concrete name if we can place it in the source without generating a template specialization
        // otherwise it's based on typeid, so we are missing a demangle function ...

        auto allocatedType = castExpr->getType()->getPointeeType();
        if (allocatedType.getTypePtr()->isTemplateTypeParmType()) {
            // TODO: somehow add compiler specific demangle ...
            // We could generate a (manual) specialization for this function, but for now, this is enough
            // Let's just use type_info::name(), and demangle it somehow runtime...
            // or maybe create an initialization code dumping the mangled names for all affected specializations? (the ones skipped by the previous return)
            macroStart += "typeid(" + allocatedType.getAsString() + ").name()";
        } else {
            macroStart += "\"" + allocatedType.getAsString() + "\""; // + (int)allocatedType.getTypePtr()->getAsTagDecl();
        }
        macroStart += ", ";

        // 2nd parameter: source location. At least this is easy
        auto& sm = result.Context->getSourceManager();
        auto ploc = sm.getPresumedLoc(newExpr->getLocEnd());
        macroStart += "\"";
        macroStart += ploc.getFilename();
        macroStart += ":";
        macroStart += std::to_string(ploc.getLine());
        macroStart += "\", ";

        // 3rd parameter: the new call. It's the expression itself

        clang::SourceLocation startLoc = newExpr->getLocStart();
        mRewriter->InsertText(startLoc, macroStart);

        // 4th parameter: sizeof type
        std::string macroEnd;
        macroEnd += ", ";
        macroEnd += "sizeof(";
        macroEnd += allocatedType.getAsString();
        macroEnd += ")";


        // 5th argument: size, which is the first argument to the call
        // TODO: extract it to a variable!
        macroEnd += ", ";
        llvm::raw_string_ostream os(macroEnd);
        newExpr->getArg(0)->printPretty(os, nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
        os.flush();

        // end added function call
        macroEnd += ")";

        clang::SourceLocation endLoc = newExpr->getLocEnd();
        mRewriter->InsertTextAfterToken(endLoc, macroEnd);
    }

    clang::StringRef OpNewExprHandler::getID() const {
        return "typegrind";
    }
}
