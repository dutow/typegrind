
#include "CompilerLikeOptionsParser.h"

#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/Action.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Driver/Job.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Tooling/CompilationDatabasePluginRegistry.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Option/Arg.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

namespace {

// Helper for recursively searching through a chain of actions and collecting
// all inputs, direct and indirect, of compile jobs.
    struct CompileJobAnalyzer {
        void run(const driver::Action *A) {
            runImpl(A, false);
        }

        SmallVector<std::string, 2> Inputs;

    private:

        void runImpl(const driver::Action *A, bool Collect) {
            bool CollectChildren = Collect;
            switch (A->getKind()) {
                case driver::Action::CompileJobClass:
                    CollectChildren = true;
                    break;

                case driver::Action::InputClass: {
                    if (Collect) {
                        const driver::InputAction *IA = cast<driver::InputAction>(A);
                        Inputs.push_back(IA->getInputArg().getSpelling());
                    }
                } break;

                default:
                    // Don't care about others
                    ;
            }

            for (driver::ActionList::const_iterator I = A->begin(), E = A->end();
                 I != E; ++I)
                runImpl(*I, CollectChildren);
        }
    };

    // TODO: copied from clang!
// Unary functor for asking "Given a StringRef S1, does there exist a string
// S2 in Arr where S1 == S2?"
    struct MatchesAny {
        MatchesAny(ArrayRef<std::string> Arr) : Arr(Arr) {}
        bool operator() (StringRef S) {
            for (const std::string *I = Arr.begin(), *E = Arr.end(); I != E; ++I)
                if (*I == S)
                    return true;
            return false;
        }
    private:
        ArrayRef<std::string> Arr;
    };

    // TODO: copied from clang!
    static bool stripPositionalArgs(std::vector<const char *> Args,
                                    std::vector<std::string> &Result) {
        IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();

        clang::TextDiagnosticPrinter *DiagClient = new clang::TextDiagnosticPrinter(llvm::errs(), new clang::DiagnosticOptions());
        clang::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID(new clang::DiagnosticIDs());
        clang::DiagnosticOptions *diagnosticOptions = new DiagnosticOptions();
        clang::DiagnosticsEngine Diags(DiagID, diagnosticOptions, DiagClient);

        //llvm::sys::Path clangPath = llvm::sys::Program::FindProgramByName("clang");
        std::unique_ptr<driver::Driver> NewDriver(new driver::Driver(
                /* ClangExecutable= */ "/usr/bin/clang", llvm::sys::getDefaultTargetTriple(),
                                       Diags));
        NewDriver->setCheckInputsExist(false);

        // This becomes the new argv[0]. The value is actually not important as it
        // isn't used for invoking Tools.
        Args.insert(Args.begin(), "clang-tool");
        Args.erase(std::remove_if(Args.begin(), Args.end(),
                                  MatchesAny(std::string("--build"))),
                   Args.end());

        const std::unique_ptr<driver::Compilation> Compilation(
                NewDriver->BuildCompilation(Args));

        const driver::JobList &Jobs = Compilation->getJobs();

        CompileJobAnalyzer CompileAnalyzer;


        for (const auto &Cmd : Jobs) {
            // Collect only for Assemble jobs. If we do all jobs we get duplicates
            // since Link jobs point to Assemble jobs as inputs.
            if (Cmd.getSource().getKind() == driver::Action::AssembleJobClass) {
                llvm::outs() << "onee\n";
                CompileAnalyzer.run(&Cmd.getSource());
            }
        }

        if (CompileAnalyzer.Inputs.empty()) {
            // No compile jobs found.
            // FIXME: Emit a warning of some kind?
            llvm::errs() << "eh\n";
            return false;
        }

        // Remove all compilation input files from the command line. This is
        // necessary so that getCompileCommands() can construct a command line for
        // each file.
        std::vector<const char *>::iterator End = std::remove_if(
                Args.begin(), Args.end(), MatchesAny(CompileAnalyzer.Inputs));

        Result = std::vector<std::string>(Args.begin() + 1, End);
        return true;
    }

    FixedCompilationDatabase *loadFromCompilerLikeCommandLine(
            int &Argc, const char *const *Argv, Twine Directory = ".") {
        std::vector<const char *> afterCommandLine(Argv, Argv+Argc);

        std::vector<std::string> StrippedArgs;
        if (!stripPositionalArgs(afterCommandLine, StrippedArgs))
            return nullptr;
        return new FixedCompilationDatabase(Directory, StrippedArgs);
    }
}

CompilerLikeOptionsParser::CompilerLikeOptionsParser(int &argc, const char **argv, llvm::cl::OptionCategory &Category,
                                                     const char *Overview) {

    static cl::list<std::string> SourcePaths(
            cl::Positional, cl::desc("<source0> [... <sourceN>]"), cl::OneOrMore,
            cl::cat(Category));

    cl::HideUnrelatedOptions(Category);

    Compilations.reset(loadFromCompilerLikeCommandLine(argc, argv));

    cl::ParseCommandLineOptions(argc, argv, Overview);
    SourcePathList = SourcePaths;

    if (!Compilations) {
        std::string ErrorMessage = "No compilation given!";
        llvm::report_fatal_error(ErrorMessage);
    }
}
