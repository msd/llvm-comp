#pragma once

#include "DeclListNode.hpp"
#include "ExternListNode.hpp"

class ProgramNode : public ASTnode
{
  public:
    ExternListNode *extern_list;
    DeclListNode *decl_list;
    ProgramNode(Parser *parser, unique_ptr<ExternListNode> eln,
                unique_ptr<DeclListNode> dln)
        : ASTnode(parser)
    {
        children.push_back(move(eln));
        children.push_back(move(dln));
        extern_list = dynamic_cast<ExternListNode *>(children[0].get());
        decl_list = dynamic_cast<DeclListNode *>(children[1].get());
    }

    virtual const string node_type() const
    {
        return "PROGRAM";
    }

    virtual Value *codegen();
};
