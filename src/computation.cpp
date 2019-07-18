/*************************************************************************
	> File Name: computation.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 09:02:17 AM UTC
 ************************************************************************/

#include "computation.h"
#include "schedule.h"
#include "util.h"
#include "ir.h"
#include "tensor.h"

namespace {


} // local namespace

namespace SC
{

void NestLoopComNode::calcu_input_tensors()
{
    for(auto& stmt : body)
    {
        switch(stmt->node_type)
        {
            case NodeType::LetStmt:
        }        
    }
}

void NestLoopComNode::calcu_output_tensors()
{
    
}

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

Stmt NestLoopComNode::buildBody() const
{
    if(body.size() == 1)
        return body[0];

    Stmt ret = Block::make(body[0], body[1]);
    for(unsigned i = 2;i < body.size(); i ++)
        ret = Block::make(ret, body[i]);
    return ret;
}


} //namespace SC

