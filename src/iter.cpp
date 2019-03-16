/*************************************************************************
	> File Name: iter.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Mar 2019 03:06:56 AM UTC
 ************************************************************************/

#include "iter.h"

namespace SC
{

Iter IterNode::make(IterType iter_type, Range range, 
        VarExpr var, IterSche iter_sche)
{
    CHECK_IF(var.notNull(), "var is null");
    IterNode* n = new IterNode();
    n->iter_type = iter_type;
    n->range = std::move(range);
    n->var = std::move(var);
    n->iter_sche = iter_sche;
    // no need to initialize ref_count, which has a proper default constructor.
    return Iter(n);
}
} // namespace SC

