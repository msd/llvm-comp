#pragma once

#include <deque>
#include <fstream>
#include <memory>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

using std::deque;
using std::ifstream;
using std::string;
using std::unique_ptr;
using std::vector;

class Tokenizer
{
    deque<unique_ptr<TOKEN>> tok_buffer;

    /**
     * @brief Reads another character from the file, if one exists.
     *
     * @return the next character in the file or EOF
     */
    int nextChar();

    /**
     * @brief Construct and return a new token at the current line and column
     * @param lexVal the lexeme of the new token
     * @param tok_type the token type of the new token
     * @return the token with the specified lexeme and type and the current
     * line and column
     */
    unique_ptr<TOKEN> returnTok(string lexVal, int tok_type);

    // Read file line by line -- or look for \n and if found add 1 to line
    // number and reset column number to 0
    /// gettok - Return the next token from standard input.
    unique_ptr<TOKEN> gettok();

    unique_ptr<ifstream> file;

    template <typename V>
    unique_ptr<TokenWithValue<V>> retTokVal(string lexVal, int tok_type,
                                            V value);

  public:
    string IdentifierStr; // Filled in if IDENT
    int IntVal;           // Filled in if INT_LIT
    bool BoolVal;         // Filled in if BOOL_LIT
    float FloatVal;       // Filled in if FLOAT_LIT
    string StringVal;     // Filled in if String Literal
    int lineNo, columnNo;

    /// CurTok/next - Provide a simple token buffer.  CurTok is the
    /// current token the parser is looking at.  next reads another
    /// token from the lexer and updates CurTok with its results.
    // TOKEN CurTok;

    Tokenizer(unique_ptr<ifstream> &&);
    ~Tokenizer();

    unique_ptr<TOKEN> next();

    void put_back(unique_ptr<TOKEN> tok);
};

void assert_tok(TOKEN_TYPE tok_type, string err_msg);

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg);
