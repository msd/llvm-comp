#pragma once

#include "DeclListNode.hpp"
#include "ExternListNode.hpp"

class ProgramNode : public ASTnode
{
  public:
    ExternListNode *extern_list;
    DeclListNode *decl_list;
    ProgramNode(Parser *parser, std::unique_ptr<ExternListNode> eln,
                std::unique_ptr<DeclListNode> dln)
        : ASTnode(parser)
    {
        children.push_back(std::move(eln));
        children.push_back(std::move(dln));
        extern_list = dynamic_cast<ExternListNode *>(children[0].get());
        decl_list = dynamic_cast<DeclListNode *>(children[1].get());
    }

    virtual std::string node_type() const
    {
        return "PROGRAM";
    }

    virtual Value *codegen();
};
