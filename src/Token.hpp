#pragma once

#include <string>

#include "TokenType.hpp"

using std::string;

// TOKEN struct is used to keep track of information about a token
struct TOKEN
{
    int type = INVALID;
    string lexeme;
    int lineNo;
    int columnNo;
    virtual ~TOKEN() = default;
};

template <class TokenVal> struct TokenWithValue : TOKEN
{
    TokenVal value;
};

struct TokenBool : TokenWithValue<bool>
{
};
struct TokenInt : TokenWithValue<int>
{
};
struct TokenFloat : TokenWithValue<float>
{
};
