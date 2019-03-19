/*************************************************************************
	> File Name: computation.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 09:02:17 AM UTC
 ************************************************************************/

#include "computation.h"
#include "schedule.h"
#include "util.h"

namespace SC
{

Computation NestLoopComNode::make(Schedule& s,
        const std::string& name,
        std::vector<Iter> root_iters,
        std::vector<Iter> reduce_iters,
        std::vector<Stmt> body)
{
    CHECK_IF(body.size()>0, "body is null");

    for(auto& iter : reduce_iters)
    {
        CHECK_IF(iter->iter_type == IterType::REDUCTION, 
                " type of reduce iter must be reduction");
    }

    NestLoopComNode* n = new NestLoopComNode();
    n->name = name;
    n->root_iters = std::move(root_iters);
    n->reduce_iters = std::move(reduce_iters);
    n->body = std::move(body);
    Computation cp(n);
    s.addComputation(cp);
    return cp;
}

Computation NestLoopComNode::make(Schedule& s,
        const std::string& name,
        std::vector<Iter> root_iters,
        std::vector<Stmt> body)
{
    CHECK_IF(body.size()>0, "body is null");

    NestLoopComNode* n = new NestLoopComNode();
    n->name = name;
    n->root_iters = std::move(root_iters);
    n->body = std::move(body);
    Computation cp(n);
    s.addComputation(cp);
    return n;
}


} //namespace SC

