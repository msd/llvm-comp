#pragma once

#include "RvalNode.hpp"

class Parser;

template <typename T> class LitNode : public RvalNode
{
  public:
    T data;
    LitNode(Parser *parser, T litVal) : RvalNode(parser)
    {
        data = litVal;
    }

    virtual std::string data_str() const
    {
        return std::to_string(data);
    }

    virtual std::string to_string() const
    {
        return node_type() + "=" + data_str();
    }

    virtual ~LitNode()
    {
    }

    virtual char expr_type() = 0;
};
