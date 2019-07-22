/*************************************************************************
	> File Name: computation.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 09:02:17 AM UTC
 ************************************************************************/

#include <vector>
#include "computation.h"
#include "schedule.h"
#include "util.h"
#include "ir.h"
#include "tensor.h"
#include "irtraveller.h"

namespace {


} // local namespace

namespace SC
{

void gather_tensors(IRFuncTraveller::TravelFunc f, const NodeRef& n)
{
    IRFuncTraveller traverller(f);
    traverller.traverse(n);
}

/*void gather_tensors_of_stmt(IRFuncTraveller::TravelFunc f, Stmt stmt)
{
    switch(stmt->node_type)
    {
        case NodeType::LET_STMT:
        {
            auto n = stmt.cast_to<LetStmt>();
            gather_tensors(f, n->value);
            gather_tensors_of_stmt(f, n->body);
            break;
        }
        case NodeType::BLOCK:
        {
            auto n = stmt.cast_to<Block>();
            gather_tensors_of_stmt(f, n->first);
            gather_tensors_of_stmt(f, n->rest);
            break;
        }
        case NodeType::DMA_LOAD:
        {
            auto n = stmt.cast_to<DMALoad>();

        }
    }
}*/

void AllocateComNode::calcu_output_tensors()
{
    std::string tensorname = name;
    if(loc == TensorLoc::MEM)
        name += "_m";
    else if(loc == TensorLoc::LDM)
        name += "_l";
    else
        CHECK_IF(false, "no such tensor location");
    Tensor t(name, shape, data_type);
    outputTensors.emplace_back(std::move(t));
}

Stmt AllocateComNode::buildBody() const
{
    const Tensor& t = outputTensors[0];
    VarExpr buffer_var(data_type, t->name);
    Stmt alloc = Allocate::make(buffer_var, 
            data_type, 
            shape,
            loc);
    return alloc;
}

Computation AllocateComNode::make(
        const std::string& name,
        std::vector<Expr> shape,
        DataType data_type,
        TensorLoc loc)
{
    auto n = new AllocateComNode(); 
    n->name = name;
    n->shape = std::move(shape);
    n->data_type = data_type;
    n->loc = loc;
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
    return cp;
}
Computation AllocateComNode::make(
        Schedule& s,
        const std::string& name,
        std::vector<Expr> shape,
        DataType data_type,
        TensorLoc loc)
{
    auto n = new AllocateComNode(); 
    n->name = name;
    n->shape = std::move(shape);
    n->data_type = data_type;
    n->loc = loc;
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
    s.addComputation(cp);
    return cp;
}


void PlaceHolderComNode::calcu_output_tensors()
{
    Tensor t(name+"_t", shape, data_type);
    outputTensors.emplace_back(std::move(t));
}

Computation PlaceHolderComNode::make(
        const std::string& name,
        std::vector<Expr> shape,
        DataType data_type)
{
    auto n = new PlaceHolderComNode();
    n->name = name;
    n->shape = std::move(shape);
    n->data_type = data_type;
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
    return cp;
}

Computation PlaceHolderComNode::make(
        Schedule& s,
        const std::string& name,
        std::vector<Expr> shape,
        DataType data_type)
{
    auto n = new PlaceHolderComNode();
    n->name = name;
    n->shape = std::move(shape);
    n->data_type = data_type;
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
    s.addComputation(cp);
    return cp;
}


void NestLoopComNode::calcu_input_tensors()
{

    std::unordered_set<Tensor> visited;
    std::vector<Tensor> tensors;
    auto func = [&visited, &tensors](const NodeRef& n){
        const Call* cn = n.cast_to<Call>();
        if(cn != nullptr && cn->call_type==CallType::TENSOR_ACCESS)
        {
            const TensorNode* tn = cn->tb.cast_to<TensorNode>();
            const Tensor& t = tn->source_cps[0]->outputTensors[tn->source_output_index];
            if(!visited.count(t))
            {
                visited.insert(t);
                tensors.push_back(t);
            }
        }
    };

    for(auto& stmt : body)
    {
        switch(stmt->node_type)
        {
            case NodeType::STORE:
            {
                gather_tensors(func, stmt.cast_to<Store>()->rhs);
                break;
            }
            case NodeType::REDUCE:
            {
                gather_tensors(func, stmt.cast_to<Reduce>()->rhs);
                break;
            }
            case NodeType::DMA_LOAD:
            {
                auto n = stmt.cast_to<DMALoad>();
                auto tn = n->src.cast_to<TensorNode>();
                auto t = tn->source_cps[0]->outputTensors[tn->source_output_index];
                if(!visited.count(t))
                {
                    visited.insert(t);
                    tensors.push_back(t);
                }
                break;
            }
            case NodeType::DMA_STORE:
            {
                auto n = stmt.cast_to<DMAStore>();
                auto tn = n->src.cast_to<TensorNode>();
                auto t = tn->source_cps[0]->outputTensors[tn->source_output_index];
                if(!visited.count(t))
                {
                    visited.insert(t);
                    tensors.push_back(t);
                }
                break;
            }
            default:
                CHECK_IF(false, "cannot handle this stmt as initial body of operation");

        }
    }
    inputTensors = std::move(tensors);

}

void NestLoopComNode::calcu_output_tensors()
{
    
    std::unordered_set<Tensor> visited;
    std::vector<Tensor> tensors;
    auto func = [&visited, &tensors](const NodeRef& n){
        const Call* cn = n.cast_to<Call>();
        if(cn != nullptr && cn->call_type==CallType::TENSOR_ACCESS)
        {
            const TensorNode* tn = cn->tb.cast_to<TensorNode>();
            LOG("name tensor %s", tn->name.c_str());
            LOG("cp size %lu", tn->source_cps.size());
            LOG("first cp name %s", tn->source_cps[0]->name.c_str());
            LOG("first cp tensor name %s", tn->source_cps[0]->outputTensors[0]->name.c_str());
            const Tensor& t = tn->source_cps[0]->outputTensors[tn->source_output_index];
            if(!visited.count(t))
            {
                visited.insert(t);
                tensors.push_back(t);
            }
        }
    };

    for(auto& stmt : body)
    {
        switch(stmt->node_type)
        {
            case NodeType::STORE:
            {
                gather_tensors(func, stmt.cast_to<Store>()->lhs);
                break;
            }
            case NodeType::REDUCE:
            {
                gather_tensors(func, stmt.cast_to<Reduce>()->lhs);
                break;
            }
            case NodeType::DMA_LOAD:
            {
                auto n = stmt.cast_to<DMALoad>();
                auto tn = n->dst.cast_to<TensorNode>();
                auto t = tn->source_cps[0]->outputTensors[tn->source_output_index];
                if(!visited.count(t))
                {
                    visited.insert(t);
                    tensors.push_back(t);
                }
                break;
            }
            case NodeType::DMA_STORE:
            {
                auto n = stmt.cast_to<DMAStore>();
                auto tn = n->dst.cast_to<TensorNode>();
                auto t = tn->source_cps[0]->outputTensors[tn->source_output_index];
                if(!visited.count(t))
                {
                    visited.insert(t);
                    tensors.push_back(t);
                }
                break;
            }
            default:
                CHECK_IF(false, "cannot handle this stmt as initial body of operation");

        }
    }
    //for(auto& t : tensors)
        //t->source_cps.push_back(*this);
    outputTensors = std::move(tensors);
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
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
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
    n->calcu_output_tensors();
    n->calcu_input_tensors();
    Computation cp(n);
    for(auto& t : n->outputTensors)
        t.add_source_computation(cp);
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

