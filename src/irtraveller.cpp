/*************************************************************************
	> File Name: irtraveller.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 18 Jul 2019 07:48:21 AM UTC
 ************************************************************************/

#include "irtraveller.h"

namespace 
{

/**
 * \bref define default visit implemenation with no actions
 * \param v, visitor class
 * \param t, Node class
 */
#define DEFAULT_VISIT_NO_OP(t) \
    void IRTraveller::visit(const t *n) { }

/**
 * \bref define default visit implemenation for single op
 * \param v, visitor class
 * \param t, Node class 
 */
#define DEFAULT_VISIT_SINGLE(t) \
    void IRTraveller::visit(const t *n) { \
        traverse(n->value); \
    }

/**
 * \bref define default visit implemenation for binary-op
 * \param v, visitor class
 * \param t, Node class 
 */
#define DEFAULT_VISIT_BINARY(t) \
    void IRTraveller::visit(const t *n) { \
        traverse(n->a); \
        traverse(n->b); \
    }

} // namespace

namespace SC
{

DEFAULT_VISIT_NO_OP(IntImm);
DEFAULT_VISIT_NO_OP(UIntImm);
DEFAULT_VISIT_NO_OP(FloatImm);
DEFAULT_VISIT_NO_OP(StringImm);

DEFAULT_VISIT_NO_OP(Variable);

DEFAULT_VISIT_SINGLE(Cast);
DEFAULT_VISIT_SINGLE(Not);

DEFAULT_VISIT_BINARY(Add);
DEFAULT_VISIT_BINARY(Sub);
DEFAULT_VISIT_BINARY(Mul);
DEFAULT_VISIT_BINARY(Div);
DEFAULT_VISIT_BINARY(Mod);
DEFAULT_VISIT_BINARY(Min);
DEFAULT_VISIT_BINARY(Max);

DEFAULT_VISIT_BINARY(Eq);
DEFAULT_VISIT_BINARY(Ne);
DEFAULT_VISIT_BINARY(Lt);
DEFAULT_VISIT_BINARY(Gt);
DEFAULT_VISIT_BINARY(Le);
DEFAULT_VISIT_BINARY(Ge);

DEFAULT_VISIT_BINARY(And);
DEFAULT_VISIT_BINARY(Or);

void IRTraveller::visit(const Select* n)
{
    traverse(n->true_case);    
    traverse(n->false_case);
}
void IRTraveller::visit(const Call* n)
{
    for(const auto& arg : n->args)
        traverse(arg);
}
void IRTraveller::visit(const LetStmt* n)
{
    traverse(n->value);
    traverse(n->body);
}
void IRTraveller::visit(const For* n)
{
    traverse(n->var);
    traverse(n->min);
    traverse(n->extent);
    traverse(n->body);
}
void IRTraveller::visit(const Block* n)
{
    traverse(n->first);
    traverse(n->rest);
}
void IRTraveller::visit(const IfThenElse* n)
{
    traverse(n->cond);
    traverse(n->then_case);
    traverse(n->else_case);
}
void IRTraveller::visit(const Store* n)
{
    traverse(n->lhs);
    traverse(n->rhs);
}
void IRTraveller::visit(const Reduce* n)
{
    traverse(n->lhs);
    traverse(n->rhs);
    for(auto& iter : n->reduce_iters)
        traverse(iter->var);
}
void IRTraveller::visit(const Evaluate* n)
{
    traverse(n->value);
}
void IRTraveller::visit(const DMALoad* n)
{
    traverse(n->src_start);
    traverse(n->src_end);
    traverse(n->dst_start);
    traverse(n->dst_end);
    /*for(auto& e : n->src_start)    
        traverse(e);
    for(auto& e : n->src_end)    
        traverse(e);
    for(auto& e : n->dst_start)    
        traverse(e);
    for(auto& e : n->dst_end)    
        traverse(e);*/
}
void IRTraveller::visit(const DMAStore* n)
{
    traverse(n->src_start);
    traverse(n->src_end);
    traverse(n->dst_start);
    traverse(n->dst_end);
    /*for(auto& e : n->src_start)    
        traverse(e);
    for(auto& e : n->src_end)    
        traverse(e);
    for(auto& e : n->dst_start)    
        traverse(e);
    for(auto& e : n->dst_end)    
        traverse(e);*/
}
void IRTraveller::visit(const Allocate* n)
{
    for(auto& e: n->shape)
        traverse(e);
}
void IRTraveller::visit(const Free* n)
{
    for(auto& e: n->shape)
        traverse(e);
}

} // namespace SC

