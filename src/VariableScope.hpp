#pragma once

#include <iostream>
#include <map>

#include "VarDeclNode.hpp"

class VariableScope
{
    VariableScope *inherited;
    // map<string, Value *> local_decls{};
    std::map<std::string, Value *> local_addrs{};
    std::map<std::string, VarDeclNode *> local_decls{};
    VariableScope(VariableScope *inherited) : inherited(inherited)
    {
    }

    auto hasLocalName(std::string var_name)
    {
        return static_cast<bool>(local_decls.count(var_name));
    }

  public:
    VariableScope() : inherited(nullptr)
    {
    }
    static auto inherit_vars(VariableScope *outer_scope)
    {
        return std::unique_ptr<VariableScope>{new VariableScope(outer_scope)};
    }

    bool hasName(std::string var_name)
    {
        return hasLocalName(var_name) ||
               (inherited && inherited->hasName(var_name));
    }

    void setAddr(std::string var_name, Value *addr);

    void setDecl(std::string var_name, VarDeclNode *decl);

    // todo check calls to getDecl can be substituted for hasLocalName
    VarDeclNode *getDecl(std::string identifier)
    {
        if (local_decls.count(identifier))
        {
            return local_decls[identifier];
        }
        if (inherited)
        {
            return inherited->getDecl(identifier);
        }
        return nullptr;
    }

    bool hasAddr(std::string var_name)
    {
        if (hasLocalName(var_name))
        {
            return local_addrs.count(var_name);
        }
        if (inherited)
        {
            return inherited->hasAddr(var_name);
        }
        return false;
    }

    Value *getAddr(std::string var_name);
};
