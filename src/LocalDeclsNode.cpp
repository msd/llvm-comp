#include "LocalDeclsNode.hpp"

#include "the_externs.hpp"

Value *LocalDeclsNode::codegen()
{
	if (children.size())
	{
		Value *v;

		for (int i = 0; i < children.size(); ++i)
		{
			v = children[i]->codegen();
			if (!v)
			{
				return nullptr;
			}
		}
		return v;
	}
	return Builder.getTrue();
}
