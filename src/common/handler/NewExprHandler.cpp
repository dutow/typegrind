
#include "NewExprHandler.h"

namespace typegrind {

    /**
     * True if placement new (we don't care about that)
     */
    bool isPlacementNew(const clang::CXXNewExpr* expr) {
        // Regular new expressions aren't placement new
        if (expr->getNumPlacementArgs() == 0)
            return false;
        return true;
    }

    /**
     * TODO: move to helper class
     * TODO: handle this differently, see examples/default_new_parametric_macro2.cpp for details
     *
     * Expands the location if it's a macro, and returns the location at the start of the original source (before expansion)
     */
    static clang::SourceLocation getLocationAtExpansionStart(clang::SourceLocation original, clang::SourceManager& sm) {
        if (original.isMacroID()) {
            auto expansionRange = sm.getImmediateExpansionRange(original);
            return expansionRange.first;
        }
        return original;
    }

    /**
     * TODO: move to helper class
     * TODO: handle this differently, see examples/default_new_parametric_macro2.cpp for details
     *
     * Expands the location if it's a macro, and returns the location at the end of the original source (before expansion)
     */
    static clang::SourceLocation getLocationAtExpansionEnd(clang::SourceLocation original, clang::SourceManager& sm) {
        if (original.isMacroID()) {
            auto expansionRange = sm.getImmediateExpansionRange(original);
            return expansionRange.second;
        }
        return original;
    }

    NewExprHandler::NewExprHandler(clang::Rewriter*& rewriter)
            : mRewriter(rewriter)
    {
    }

    void NewExprHandler::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
    {
        const clang::CXXNewExpr* newExpr = result.Nodes.getNodeAs<clang::CXXNewExpr>("newStmt");
        if (nullptr == newExpr)
        {
            llvm::errs() << "Couldn't convert MatcherResult to CXXNewExpr!\n";
            return;
        }
        if (!isPlacementNew(newExpr))
        {
            // Skipping substituted template types
            auto srcType = newExpr->getAllocatedTypeSourceInfo()->getTypeLoc();
            auto ptr = srcType.getType().getTypePtr()->getAs<clang::SubstTemplateTypeParmType>();
            if (ptr) {
                // This is an automatic template specialization.
                // TODO: add additional checks that this condition is good! TBH I have no idea why it works, I should read more CLANG apidocs :)
                // I mean:
                // * partial template specialization
                // * why isn't a specialized template a substitution? (or maybe it's in the standard?)
                return;
            }

            std::string macroStart = "TYPEGRIND_LOG_ALLOC";
            if (newExpr->isArray()) {
                macroStart += "_ARRAY";
            }
            macroStart += "(";

            // 1st paramter: name of the type.
            // Currently it is the concrete name if we can place it in the source without generating a template specialization
            // otherwise it's based on typeid, so we are missing a demangle function ...

            auto allocatedType = newExpr->getAllocatedType();
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

            clang::SourceLocation startLoc = getLocationAtExpansionStart(newExpr->getStartLoc(), sm);
            mRewriter->InsertText(startLoc, macroStart);

            // 4th parameter: sizeof type
            std::string macroEnd;
            macroEnd += ", ";
            macroEnd += "sizeof(";
            macroEnd += allocatedType.getAsString();
            macroEnd += ")";


            // if this is as array, there is an 5th parameter, the array size
            if (newExpr->isArray()) {
                macroEnd += ", ";
                llvm::raw_string_ostream os(macroEnd);
                // TODO: this assumes that the size expression is const, which isn't always true! => extract expression to a variable
                newExpr->getArraySize()->printPretty(os, nullptr, clang::PrintingPolicy(result.Context->getPrintingPolicy()));
                os.flush();
            }


            // end added function call
            macroEnd += ")";

            clang::SourceLocation endLoc = getLocationAtExpansionEnd(newExpr->getEndLoc(), sm);
            mRewriter->InsertTextAfterToken(endLoc, macroEnd);
        }
    }

    clang::StringRef NewExprHandler::getID() const {
        return "typegrind";
    }

}
