#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"

#include "ASTnode.hpp"
#include "FunDefNode.hpp"
#include "Parser.hpp"
#include "ProgramNode.hpp"
#include "my_errors.hpp"

using namespace llvm;
using namespace llvm::sys;

LLVMContext TheContext;
IRBuilder<> Builder{TheContext};

// Make the module, which holds all the code.
std::unique_ptr<Module> TheModule =
    std::make_unique<Module>("mini-c", TheContext);

std::map<std::string, FunctionSignature *> ExternedFunctions;
std::map<std::string, FunDefNode *> DefinedFunctions;
std::stack<VariableScope *> ActiveScopes;

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Recursive Descent Parser - Function call for each production
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// AST Printer
//===----------------------------------------------------------------------===//

void write_header(std::string path)
{
    std::ifstream file{path};
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const ASTnode &ast)
{
    os << ast.to_string();
    return os;
}

static void print_ast(ASTnode *root, int level = 0)
{
    std::string pre{};

    for (int i = 0; i < level; ++i)
    {
        pre += '\t';
    }
    std::cout << pre << root->to_string() << '\n';
    if (root->children.empty())
    {
        for (int i = 0; i < root->children.size(); ++i)
        {
            print_ast(root->children[i].get(), level + 1);
        }
    }
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

std::optional<size_t> str_rfind(std::string const &haystack, char needle)
{
    auto i = haystack.rfind(needle);
    if (i == std::string::npos)
    {
        return {};
    }
    return i;
}

inline std::string without_no_extension(std::string path)
{
    return str_rfind(path, '.')
        .transform([&path](size_t i) { return path.substr(0, i); })
        .value_or(path);
    ;
}

struct program_args
{
    std::string file_path{};
    std::optional<std::string> output_path_provided{};

    // returns the input file path with the extension changed to '.ll'
    // depends on file_path
    [[nodiscard]] std::string default_output_path() const
    {
        return without_no_extension(file_path) + ".ll";
    }

    [[nodiscard]] std::string output_path() const
    {
        return output_path_provided.value_or(default_output_path());
    }
};

std::optional<program_args> parse_args(std::span<char *> argStrings)
{
    if (argStrings.size() != 2)
    {
        return {};
    }
    std::string file_path{argStrings[1]};

    return program_args{
        .file_path = file_path,
        .output_path_provided = {},
    };
}

int main2(program_args args)
{
    auto globalScope = VariableScope();

    ActiveScopes.push(&globalScope);

    auto source_file = std::make_unique<std::ifstream>(args.file_path);
    Tokenizer tok{std::move(source_file)};

    Parser parser{&tok};

    // Run the parser now.
    try
    {
        std::unique_ptr<ProgramNode> program = parser.parse_program();
        std::cerr << "Parsing Finished\n";
        print_ast(program.get());
        program->codegen();
    }
    catch (syntax_error &e)
    {
        std::cerr << "Parsing ERROR on line " << e.lineNo << " column "
                  << tok.columnNo << " received token " << e.erroneous_token
                  << std::endl
                  << e.what() << '\n';
    }
    catch (semantic_error &e)
    {
        std::cerr << "Semantic ERROR on line " << e.lineNo << " column "
                  << tok.columnNo << " received token " << e.erroneous_token
                  << std::endl
                  << e.what() << '\n';
    }
    catch (compiler_error &e)
    {
        std::cerr << "Compiler ERROR on line " << e.lineNo << " column "
                  << tok.columnNo << " received token " << e.erroneous_token
                  << std::endl
                  << e.what() << '\n';
    }
    catch (std::exception &e)
    {
        std::cerr << "unknown error: " << e.what() << std::endl;
    }

    //********************* Start printing final IR **************************
    // Print out all of the generated code into a file called output.ll
    std::error_code EC;
    raw_fd_ostream dest(args.output_path(), EC, sys::fs::OF_None);

    if (EC)
    {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }
    TheModule->print(errs(), nullptr); // print IR to terminal
    TheModule->print(dest, nullptr);
    //********************* End printing final IR ****************************

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{

    int const constexpr ERR_INVALID_ARGUMENTS = 2;
    auto args = parse_args(std::span{argv, static_cast<size_t>(argc)});

    if (!args)
    {
        std::string compilerPath =
            argc > 1 && argv[0] != nullptr ? argv[0] : "my_compiler";
        std::cout << "Usage: " << compilerPath << " InputFile\n";
        return ERR_INVALID_ARGUMENTS;
    }

    try
    {
        return main2(*args);
    }
    catch (std::exception &ex)
    {
        std::cerr << "[E] Error: Unhandled exception\n"
                  << ex.what() << std::endl;
    }
    return EXIT_FAILURE;
}
