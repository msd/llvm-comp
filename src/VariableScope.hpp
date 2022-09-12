#pragma once

#include <iostream>
#include <map>

#include "VarDeclNode.hpp"

using std::cout;
using std::endl;
using std::map;

class VariableScope
{
    VariableScope *inherited;
    // map<string, Value *> local_decls{};
    map<string, Value *> local_addrs{};
    map<string, VarDeclNode *> local_decls{};
    VariableScope(VariableScope *inherited) : inherited(inherited)
    {
    }

  public:
    VariableScope() : inherited(nullptr)
    {
    }
    static auto inherit_vars(VariableScope *outer_scope)
    {
        return unique_ptr<VariableScope>{new VariableScope(outer_scope)};
    }

    auto hasLocalName(string var_name)
    {
        return static_cast<bool>(local_decls.count(var_name));
    }

    bool hasName(string var_name)
    {
        if (!(hasLocalName(var_name) ||
              (inherited && inherited->hasName(var_name))))
        {
            cout << "DECL LOOKUP FAILED: " << var_name << endl;
        }
        return hasLocalName(var_name) ||
               (inherited && inherited->hasName(var_name));
    }

    void setAddr(string var_name, Value *addr);

    void setDecl(string var_name, VarDeclNode *decl);

    // todo check calls to getDecl can be substituted for hasLocalName
    VarDeclNode *getDecl(string identifier)
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

    bool hasAddr(string var_name)
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

    Value *getAddr(string var_name);
};
