/*************************************************************************
	> File Name: irmutator.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 08 Mar 2019 04:24:24 AM UTC
 ************************************************************************/

#include "irmutator.h"
#include <memory>
#include <vector>

namespace
{

#define DEFINE_DEFAULT_MUTATE_VISIT(node) \
    Expr IRMutator::mutateNode(const node* n) { \
        return n; \
    }


#define DEFINE_BINARY_MUTATE_VISIT(node) \
    Expr IRMutator::mutateNode(const node* n) { \
        Expr new_a = mutate(n->a); \
        Expr new_b = mutate(n->b); \
        if(new_a.sameAs(n->a) && new_b.sameAs(n->b)) \
            return n; \
        return node::make(std::move(new_a), std::move(new_b)); \
    }

} // namespace

namespace SC
{

DEFINE_DEFAULT_MUTATE_VISIT(IntImm);
DEFINE_DEFAULT_MUTATE_VISIT(UIntImm);
DEFINE_DEFAULT_MUTATE_VISIT(FloatImm);
DEFINE_DEFAULT_MUTATE_VISIT(StringImm);
DEFINE_DEFAULT_MUTATE_VISIT(Variable);

Expr IRMutator::mutateNode(const Cast* n)
{
    Expr new_value = mutate(n->value);
    if(new_value.sameAs(n->value))
        return n;
    return Cast::make(n->data_type, std::move(new_value));
}

DEFINE_BINARY_MUTATE_VISIT(Add);
DEFINE_BINARY_MUTATE_VISIT(Sub);
DEFINE_BINARY_MUTATE_VISIT(Mul);
DEFINE_BINARY_MUTATE_VISIT(Div);
DEFINE_BINARY_MUTATE_VISIT(Mod);
DEFINE_BINARY_MUTATE_VISIT(Min);
DEFINE_BINARY_MUTATE_VISIT(Max);
DEFINE_BINARY_MUTATE_VISIT(Eq);
DEFINE_BINARY_MUTATE_VISIT(Ne);
DEFINE_BINARY_MUTATE_VISIT(Lt);
DEFINE_BINARY_MUTATE_VISIT(Gt);
DEFINE_BINARY_MUTATE_VISIT(Le);
DEFINE_BINARY_MUTATE_VISIT(Ge);
DEFINE_BINARY_MUTATE_VISIT(And);
DEFINE_BINARY_MUTATE_VISIT(Or);

Expr IRMutator::mutateNode(const Not* n)
{
    Expr new_value = mutate(n->value);
    if(new_value.sameAs(n->value))
        return n;
    return Not::make(std::move(new_value));
}

/**
 * \bref mutate all the args of Call function
 */
Expr IRMutator::mutateNode(const Call* n)
{
    std::vector<Expr> new_args;
    bool not_mutated = true;
    for(const auto& arg : n->args)
    {
        Expr new_arg = mutate(arg);
        if(!new_arg.sameAs(arg))
            not_mutated = false;
        new_args.emplace_back(std::move(new_arg));
    }

    if(not_mutated) 
        return n;
    return Call::make(n->data_type, 
            n->call_type, 
            n->name,
            std::move(new_args),
            n->func);
}

Stmt IRMutator::mutateNode(const LetStmt* n)
{
    Expr new_value = mutate(n->value);   
    Stmt new_body = mutate(n->body);
    if(new_value.sameAs(n->value) && new_body.sameAs(n->body))
        return n;
    return LetStmt::make(n->var, new_value, new_body);
}

Stmt IRMutator::mutateNode(const Select* n)
{
    Expr new_cond = mutate(n->cond);
    Expr new_true_case = mutate(n->true_case);
    Expr new_false_case = mutate(n->false_case);
    if(new_cond.sameAs(n->cond) 
            && new_true_case.sameAs(n->true_case)
            && new_false_case.sameAs(n->false_case))
        return n;
    return Select::make(new_cond, new_true_case, new_false_case);
}
Stmt IRMutator::mutateNode(const For* n)
{
    Expr new_min = mutate(n->min);
    Expr new_extent = mutate(n->extent);
    Stmt new_body = mutate(n->body);
    if(new_min.sameAs(n->min) 
            && new_extent.sameAs(n->extent)
            && new_body.sameAs(n->body))
        return n;
    return For::make(n->for_type, n->var, new_min, new_extent, new_body);
}
Stmt IRMutator::mutateNode(const Block* n)
{
    Stmt new_first = mutate(n->first);
    Stmt new_rest = mutate(n->rest);
    if(new_first.sameAs(n->first) && new_rest.sameAs(n->rest))
        return n;
    return Block::make(new_first, new_rest);
}
Stmt IRMutator::mutateNode(const IfThenElse* n)
{
    Expr new_cond = mutate(n->cond);
    Stmt new_then_case = mutate(n->then_case);
    Stmt new_else_case = mutate(n->else_case);
    if(new_cond.sameAs(n->cond)
            && new_then_case.sameAs(n->then_case)
            && new_else_case.sameAs(n->else_case))
        return n;
    return IfThenElse::make(new_cond, new_then_case, new_else_case);
}

Stmt IRMutator::mutateNode(const Store* n)
{
    Expr new_lhs = mutate(n->lhs);
    Expr new_rhs = mutate(n->rhs);
    if(new_lhs.sameAs(n->lhs) && new_rhs.sameAs(n->rhs))
        return n;
    else
        return Store::make(new_lhs, new_rhs);
}

Stmt IRMutator::mutateNode(const Reduce* n)
{
    Expr new_lhs = mutate(n->lhs);
    Expr new_rhs = mutate(n->rhs);
    if(new_lhs.sameAs(n->lhs) && new_rhs.sameAs(n->rhs))
        return n;
    else
        return Reduce::make(n->reduce_type, 
                new_lhs, new_rhs, 
                n->reduce_iters);
}

Stmt IRMutator::mutateNode(const Evaluate* n)
{
    Expr new_value = mutate(n->value);
    if(new_value.sameAs(n->value))
        return n;
    else
        return Evaluate::make(new_value);
}

} // namespace SC

