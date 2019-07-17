/*************************************************************************
	> File Name: node.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 06 Mar 2019 06:37:40 AM UTC
 ************************************************************************/

#include "node.h"
#include "ir.h"
#include "irvisitor.h"
#include "irmutator.h"


namespace 
{

#define DEFINE_EXPR_VISIT(n) \
    template <> void ExprNode<n>::accept(IRVisitor* v) const \
    { \
        v->visit((const n *)this); \
    }

#define DEFINE_STMT_VISIT(n) \
    template <> void StmtNode<n>::accept(IRVisitor* v) const \
    { \
        v->visit((const n *)this); \
    }


#define DEFINE_EXPR_MUTATE(n) \
    template <> Expr ExprNode<n>::mutate_expr(IRMutator* v) const \
    { \
        return v->mutateNode((const n *)this); \
    }

#define DEFINE_STMT_MUTATE(n) \
    template <> Stmt StmtNode<n>::mutate_stmt(IRMutator* v) const \
    { \
        return v->mutateNode((const n *)this); \
    }

} // namespace 

namespace SC
{

DEFINE_EXPR_VISIT(IntImm);
DEFINE_EXPR_VISIT(UIntImm);
DEFINE_EXPR_VISIT(FloatImm);
DEFINE_EXPR_VISIT(StringImm);
DEFINE_EXPR_VISIT(Cast);
DEFINE_EXPR_VISIT(Variable);
DEFINE_EXPR_VISIT(Add);
DEFINE_EXPR_VISIT(Sub);
DEFINE_EXPR_VISIT(Mul);
DEFINE_EXPR_VISIT(Div);
DEFINE_EXPR_VISIT(Mod);
DEFINE_EXPR_VISIT(Min);
DEFINE_EXPR_VISIT(Max);
DEFINE_EXPR_VISIT(Eq);
DEFINE_EXPR_VISIT(Ne);
DEFINE_EXPR_VISIT(Lt);
DEFINE_EXPR_VISIT(Gt);
DEFINE_EXPR_VISIT(Le);
DEFINE_EXPR_VISIT(Ge);
DEFINE_EXPR_VISIT(And);
DEFINE_EXPR_VISIT(Or);
DEFINE_EXPR_VISIT(Not);
DEFINE_EXPR_VISIT(Call);
DEFINE_STMT_VISIT(LetStmt);
DEFINE_STMT_VISIT(Select);
DEFINE_STMT_VISIT(For);
DEFINE_STMT_VISIT(Block);
DEFINE_STMT_VISIT(IfThenElse);
DEFINE_STMT_VISIT(Store);
DEFINE_STMT_VISIT(Reduce);
DEFINE_STMT_VISIT(Evaluate);
DEFINE_STMT_VISIT(DMALoad);
DEFINE_STMT_VISIT(DMAStore);
DEFINE_STMT_VISIT(Allocate);
DEFINE_STMT_VISIT(Free);


DEFINE_EXPR_MUTATE(IntImm);
DEFINE_EXPR_MUTATE(UIntImm);
DEFINE_EXPR_MUTATE(FloatImm);
DEFINE_EXPR_MUTATE(StringImm);
DEFINE_EXPR_MUTATE(Cast);
DEFINE_EXPR_MUTATE(Variable);
DEFINE_EXPR_MUTATE(Add);
DEFINE_EXPR_MUTATE(Sub);
DEFINE_EXPR_MUTATE(Mul);
DEFINE_EXPR_MUTATE(Div);
DEFINE_EXPR_MUTATE(Mod);
DEFINE_EXPR_MUTATE(Min);
DEFINE_EXPR_MUTATE(Max);
DEFINE_EXPR_MUTATE(Eq);
DEFINE_EXPR_MUTATE(Ne);
DEFINE_EXPR_MUTATE(Lt);
DEFINE_EXPR_MUTATE(Gt);
DEFINE_EXPR_MUTATE(Le);
DEFINE_EXPR_MUTATE(Ge);
DEFINE_EXPR_MUTATE(And);
DEFINE_EXPR_MUTATE(Or);
DEFINE_EXPR_MUTATE(Not);
DEFINE_EXPR_MUTATE(Call);
DEFINE_STMT_MUTATE(LetStmt);
DEFINE_STMT_MUTATE(Select);
DEFINE_STMT_MUTATE(For);
DEFINE_STMT_MUTATE(Block);
DEFINE_STMT_MUTATE(IfThenElse);
DEFINE_STMT_MUTATE(Store);
DEFINE_STMT_MUTATE(Reduce);
DEFINE_STMT_MUTATE(Evaluate);
DEFINE_STMT_MUTATE(DMALoad);
DEFINE_STMT_MUTATE(DMAStore);
DEFINE_STMT_MUTATE(Allocate);
DEFINE_STMT_MUTATE(Free);
} // namespace SC
