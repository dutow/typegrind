
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "AllocationDecoratorAction.h"

using namespace clang::tooling;
using namespace llvm;


// TODO: check these static vars

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory TypegrindCategory("typegrind");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");


int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, TypegrindCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    clang::LangOptions DefaultLangOptions;
    IntrusiveRefCntPtr<clang::DiagnosticOptions> DiagOpts = new clang::DiagnosticOptions();
    clang::TextDiagnosticPrinter DiagnosticPrinter(llvm::errs(), &*DiagOpts);
    clang::DiagnosticsEngine Diagnostics(IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()), &*DiagOpts, &DiagnosticPrinter, false);
    clang::SourceManager Sources(Diagnostics, Tool.getFiles());
    clang::Rewriter rewriter(Sources, DefaultLangOptions);

    typegrind::AllocationDecoratorAction action(rewriter);
    auto factory = newFrontendActionFactory(&action);
    int result = Tool.run(factory.get());

    if (result == 0) {
        // success !
        // TODO: add command line options for different settings
        // e. g. just overwriting in memory and running clang with it as an overlay
        rewriter.overwriteChangedFiles();
    }
}
