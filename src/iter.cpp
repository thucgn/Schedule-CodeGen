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

/*Iter::Iter(const std::string& label,
        Range range, IterType iter_type)
{
    IterNode* n = new IterNode();
    Var v(label);
    n->iter_type = iter_type;
    n->range = std::move(range);
    n->var = std::move(v);
    n->iter_sche = IterSche::NO_SCHEDULE;
    this->ptr = n;
}*/

/*Iter::Iter(const std::string& label,
        std::array<Expr, 2> range, IterType iter_type)
{
    IterNode* n = new IterNode();
    Var v(label);
    n->iter_type = iter_type;
    n->range = Range(range[0], range[1]);
    n->var = std::move(v);
    n->iter_sche = IterSche::NO_SCHEDULE;
    this->ptr = n;
}*/
/*Iter::Iter(const std::string& label,
        Expr extent, IterType iter_type)
{
    IterNode* n = new IterNode();
    Var v(label);
    n->iter_type = iter_type;
    n->range = Range(0, extent);
    n->var = std::move(v);
    n->iter_sche = IterSche::NO_SCHEDULE;
    this->ptr = n;
}*/
} // namespace SC

