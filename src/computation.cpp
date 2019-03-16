/*************************************************************************
	> File Name: computation.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 09:02:17 AM UTC
 ************************************************************************/

#include "computation.h"
#include "util.h"

namespace SC
{

Computation NestLoopComNode::make(const std::string& name,
        std::vector<Iter> root_iters,
        std::vector<Iter> reduce_iters,
        std::vector<Expr> body)
{
    CHECK_IF(body.size()>0, "body is null");

    NestLoopComNode* n = new NestLoopComNode();
    n->name = name;
    n->root_iters = std::move(root_iters);
    n->reduce_iters = std::move(reduce_iters);
    n->body = std::move(body);
    return n;
}
Computation NestLoopComNode::make(const std::string& name,
        std::vector<Iter> root_iters,
        std::vector<Expr> body)
{
    CHECK_IF(body.size()>0, "body is null");

    NestLoopComNode* n = new NestLoopComNode();
    n->name = name;
    n->root_iters = std::move(root_iters);
    n->body = std::move(body);
    return n;
}


} //namespace SC

