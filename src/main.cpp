
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "AllocationDecoratorAction.h"

using namespace clang::tooling;
using namespace llvm;


static llvm::cl::OptionCategory typegrindCategory("typegrind");
static cl::opt<bool> typegrindOptBuildCode("build", cl::desc("Build the modified code automatically with clang."), cl::cat(typegrindCategory));
static cl::opt<std::string> typegrindOptSave("save", cl::desc("Save the modified files according to the specified mapping."), cl::value_desc("Mapping file"), cl::cat(typegrindCategory));
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp(R"(
Typegrind preprocesses C++ source files and instruments them with type preserving heap logging instructions.

it can run clang with in-memory modified files, or it can save the modifications to a specified location for other compilers / other versions of clang.

The expected mapping is a JSON file, for example:

{
  "/usr/include": "/some/other/place/include",
  "/just/a/file.cpp": "some/relative/location/to/the/json",
  "relative/to/the/json/location": "/to/an/absolute/path",
  "another/folder": "<discard>",
  "/a/b/c": "<overwrite>"
}
)");

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, typegrindCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    if (typegrindOptSave == "" && !typegrindOptBuildCode) {
        llvm::errs() << "--build or --save has to be specified, without them there would be no visible result of this tool.\n";
        llvm::errs() << "\nExiting.\n";
        return -1;
    }

    if (typegrindOptSave != "") {
        // check the integrity of the json config
    }

    clang::Rewriter* rewriter = nullptr;

    typegrind::AllocationDecoratorAction action(rewriter);
    auto factory = newFrontendActionFactory(&action);
    int result = Tool.run(factory.get());

    if (result == 0 && rewriter != nullptr) {
        if (typegrindOptSave != "") {
            // save the files. If we have to build too, this is probably the faster

        }
        if (typegrindOptBuildCode) {
            // let's build it!
        }
        rewriter->overwriteChangedFiles();
        delete rewriter;
    }
}
