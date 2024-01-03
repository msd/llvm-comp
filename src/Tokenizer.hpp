#pragma once

#include <deque>
#include <fstream>
#include <memory>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

class Tokenizer
{
    std::deque<std::unique_ptr<TOKEN>> tok_buffer;

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
    std::unique_ptr<TOKEN> returnTok(std::string lexVal, int tok_type);

    // Read file line by line -- or look for \n and if found add 1 to line
    // number and reset column number to 0
    /// gettok - Return the next token from standard input.
    std::unique_ptr<TOKEN> gettok();

    std::unique_ptr<std::ifstream> file;

    template <typename V>
    std::unique_ptr<TokenWithValue<V>> retTokVal(std::string lexVal,
                                                 int tok_type, V value);

  public:
    std::string IdentifierStr; // Filled in if IDENT
    int IntVal;                // Filled in if INT_LIT
    bool BoolVal;              // Filled in if BOOL_LIT
    float FloatVal;            // Filled in if FLOAT_LIT
    std::string StringVal;     // Filled in if String Literal
    int lineNo, columnNo;

    /// CurTok/next - Provide a simple token buffer.  CurTok is the
    /// current token the parser is looking at.  next reads another
    /// token from the lexer and updates CurTok with its results.
    // TOKEN CurTok;

    Tokenizer(std::unique_ptr<std::ifstream> &&);
    ~Tokenizer();

    std::unique_ptr<TOKEN> next();

    void put_back(std::unique_ptr<TOKEN> tok);
};
