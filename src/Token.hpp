#pragma once

#include <string>

using std::string;

// TOKEN struct is used to keep track of information about a token
struct TOKEN
{
    int type = -100;
    string lexeme;
    int lineNo;
    int columnNo;
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
