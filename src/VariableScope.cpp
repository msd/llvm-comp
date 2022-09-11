#include "VariableScope.hpp"

#include "my_errors.hpp"

void VariableScope::setAddr(string var_name, Value *addr)
{
    if (hasLocalName(var_name))
    {
        local_addrs[var_name] = addr;
    }
    else if (inherited)
    {
        inherited->setAddr(var_name, addr);
    }
    else
    {
        throw compiler_error("COMPILER ERROR: 452");
    }
}

Value *VariableScope::getAddr(string var_name)
{
    if (hasLocalName(var_name))
    {
        auto i = local_addrs.find(var_name);
        return i != local_addrs.end() ? i->second : nullptr;
    }
    if (inherited)
    {
        return inherited->getAddr(var_name);
    }
    return nullptr;
}

void VariableScope::setDecl(string var_name, VarDeclNode *decl)
{
    if (hasLocalName(var_name))
    {
        throw semantic_error(string() + "attempted to define " + var_name +
                                 " but has been already defined in this scope",
                             &decl->token);
    }
    local_decls[var_name] = decl;
}
