#include <fstream>
#include <iostream>
#include <map>
#include <stack>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"

#include "ASTnode.hpp"
#include "AssignmentExpr.hpp"
#include "AssignmentLHS.hpp"
#include "BoolNode.hpp"
#include "DeclNode.hpp"
#include "DisCon.hpp"
#include "FloatNode.hpp"
#include "FunCallNode.hpp"
#include "FunDeclNode.hpp"
#include "FunctionSignature.hpp"
#include "IfStmt.hpp"
#include "IfWithElseNode.hpp"
#include "IntNode.hpp"
#include "LocalDeclsNode.hpp"
#include "NegationNode.hpp"
#include "NotNode.hpp"
#include "NullStmt.hpp"
#include "Ops.hpp"
#include "ParenExprNode.hpp"
#include "Parser.hpp"
#include "ProgramNode.hpp"
#include "ReturnNothingNode.hpp"
#include "ReturnValueNode.hpp"
#include "StmtListNode.hpp"
#include "Tokenizer.hpp"
#include "ValidTypes.hpp"
#include "VarExprNode.hpp"
#include "WhileStmt.hpp"
#include "coercion.hpp"
#include "my_errors.hpp"

using namespace llvm;
using namespace llvm::sys;

using std::cerr;
using std::cout;
using std::deque;
using std::endl;
using std::exception;
using std::make_unique;
using std::map;
using std::move;
using std::stack;
using std::string;
using std::unique_ptr;
using std::vector;

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

// Make the module, which holds all the code.
unique_ptr<Module> TheModule = make_unique<Module>("mini-c", TheContext);

map<string, FunctionSignature *> ExternedFunctions;
map<string, FunDeclNode *> DefinedFunctions;
stack<VariableScope *> ActiveScopes;

// Todo whilestmnt paramnode codegen

// todo handle empty function body when codegening

// todo type checking conforming i.e. integers + floating point -> float + float
// (coercion)s|

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

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const ASTnode &ast)
{
    os << ast.to_string();
    return os;
}

static void print_ast(ASTnode *root, int level = 0)
{
    string pre = "";

    for (int i = 0; i < level; ++i)
    {
        pre += '\t';
    }
    cout << pre << root->to_string() << '\n';
    if (root->children.size())
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

int main(int argc, char **argv)
{
    auto globalScope = VariableScope();

    ActiveScopes.push(&globalScope);

    if (argc != 2)
    {
        string compilerPath = argv[0];
        cout << "Usage: " << compilerPath << " InputFile\n";
        return 1;
    }

    string file_path = argv[1];
    auto source_file = make_unique<ifstream>(file_path);
    Tokenizer tok{move(source_file)};

    Parser parser{&tok};

    // Run the parser now.
    try
    {
        unique_ptr<ProgramNode> program = parser.parse_program();
        cerr << "Parsing Finished\n";
        print_ast(program.get());
        program->codegen();
    }
    catch (syntax_error &e)
    {
        cerr << "Parsing ERROR on line " << e.lineNo << " column "
             << tok.columnNo << " received token " << e.erroneous_token << endl
             << e.what() << '\n';
    }
    catch (semantic_error &e)
    {
        cerr << "Semantic ERROR on line " << e.lineNo << " column "
             << tok.columnNo << " received token " << e.erroneous_token << endl
             << e.what() << '\n';
    }
    catch (compiler_error &e)
    {
        cerr << "Compiler ERROR on line " << e.lineNo << " column "
             << tok.columnNo << " received token " << e.erroneous_token << endl
             << e.what() << '\n';
    }

    //********************* Start printing final IR **************************
    // Print out all of the generated code into a file called output.ll
    auto Filename = "output.ll";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

    if (EC)
    {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }
    // TheModule->print(errs(), nullptr); // print IR to terminal
    TheModule->print(dest, nullptr);
    //********************* End printing final IR ****************************

    return 0;
}
