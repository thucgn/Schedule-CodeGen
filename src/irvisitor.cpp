/*************************************************************************
	> File Name: irvisitor.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 06:40:13 AM UTC
 ************************************************************************/

#include "irvisitor.h"

namespace 
{

/**
 * \bref define default visit implemenation with no actions
 * \param v, visitor class
 * \param t, Node class
 */
#define DEFAULT_VISIT_NO_OP(v, t) \
    void v::visit(const t *n) { }

/**
 * \bref define default visit implemenation for single op
 * \param v, visitor class
 * \param t, Node class 
 */
#define DEFAULT_VISIT_SINGLE(v, t) \
    void v::visit(const t *n) { \
        n->value.accept(this); \
    }

/**
 * \bref define default visit implemenation for binary-op
 * \param v, visitor class
 * \param t, Node class 
 */
#define DEFAULT_VISIT_BINARY(v, t) \
    void v::visit(const t *n) { \
        n->a.accept(this); \
        n->b.accept(this); \
    }

} // namespace

namespace SC
{

/**
 * expression node
 */

DEFAULT_VISIT_NO_OP(IRVisitor, IntImm);
DEFAULT_VISIT_NO_OP(IRVisitor, UIntImm);
DEFAULT_VISIT_NO_OP(IRVisitor, FloatImm);
DEFAULT_VISIT_NO_OP(IRVisitor, StringImm);

DEFAULT_VISIT_NO_OP(IRVisitor, Variable);

DEFAULT_VISIT_SINGLE(IRVisitor, Cast);
DEFAULT_VISIT_SINGLE(IRVisitor, Not);

DEFAULT_VISIT_BINARY(IRVisitor, Add);
DEFAULT_VISIT_BINARY(IRVisitor, Sub);
DEFAULT_VISIT_BINARY(IRVisitor, Mul);
DEFAULT_VISIT_BINARY(IRVisitor, Div);
DEFAULT_VISIT_BINARY(IRVisitor, Mod);
DEFAULT_VISIT_BINARY(IRVisitor, Min);
DEFAULT_VISIT_BINARY(IRVisitor, Max);

DEFAULT_VISIT_BINARY(IRVisitor, Eq);
DEFAULT_VISIT_BINARY(IRVisitor, Ne);
DEFAULT_VISIT_BINARY(IRVisitor, Lt);
DEFAULT_VISIT_BINARY(IRVisitor, Gt);
DEFAULT_VISIT_BINARY(IRVisitor, Le);
DEFAULT_VISIT_BINARY(IRVisitor, Ge);

DEFAULT_VISIT_BINARY(IRVisitor, And);
DEFAULT_VISIT_BINARY(IRVisitor, Or);


/**
 * \bref just visit the arguments
 */
void IRVisitor::visit(const Call* n)
{
    for(const auto& arg : n->args)
        arg.accept(this);
}

/**
 * statement node
 */

void IRVisitor::visit(const LetStmt* n)
{
    n->value.accept(this);
    n->body.accept(this);
}

void IRVisitor::visit(const Select* n)
{
    n->true_case.accept(this);
    n->false_case.accept(this);
}

void IRVisitor::visit(const For* n)
{
    n->var.accept(this);
    n->min.accept(this);
    n->extent.accept(this);
    n->body.accept(this);
}

void IRVisitor::visit(const Block* n)
{
    n->first.accept(this);
    n->rest.accept(this);
}

void IRVisitor::visit(const IfThenElse* n)
{
    n->cond.accept(this);
    n->then_case.accept(this);
    n->else_case.accept(this);
}

void IRVisitor::visit(const Store* n)
{
    n->lhs.accept(this);
    n->rhs.accept(this);
}

void IRVisitor::visit(const Reduce* n)
{
    n->lhs.accept(this);
    n->rhs.accept(this);
    for(auto& iter : n->reduce_iters)
        iter->var.accept(this);
}

void IRVisitor::visit(const Evaluate* n)
{
    n->value.accept(this);
}

void IRVisitor::visit(const DMALoad* n)
{
    n->src_start.accept(this);
    n->src_end.accept(this);
    n->dst_start.accept(this);
    n->dst_end.accept(this);
    /*for(auto& e : n->src_start)    
        e.accept(this);
    for(auto& e : n->src_end)    
        e.accept(this);
    for(auto& e : n->dst_start)    
        e.accept(this);
    for(auto& e : n->dst_end)    
        e.accept(this);*/
}
void IRVisitor::visit(const DMAStore* n)
{
    n->src_start.accept(this);
    n->src_end.accept(this);
    n->dst_start.accept(this);
    n->dst_end.accept(this);
    /*for(auto& e : n->src_start)    
        e.accept(this);
    for(auto& e : n->src_end)    
        e.accept(this);
    for(auto& e : n->dst_start)    
        e.accept(this);
    for(auto& e : n->dst_end)    
        e.accept(this);*/
}
void IRVisitor::visit(const Allocate* n)
{
    for(auto& e : n->shape)
        e.accept(this);
}
void IRVisitor::visit(const Free* n)
{
    for(auto& e : n->shape)
        e.accept(this);
}

} // namespace SC

