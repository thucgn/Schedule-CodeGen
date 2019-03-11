/*************************************************************************
	> File Name: node.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 06 Mar 2019 06:37:40 AM UTC
 ************************************************************************/

#include "node.h"
#include "ir.h"
#include "irvisitor.h"


namespace 
{

#define DEFINE_EXPR_VISIT(n) \
    template <> void ExprNode<n>::accept(IRVisitor* v) const \
    { \
        return v->visit((const n *)v); \
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
DEFINE_EXPR_VISIT(LetStmt);
DEFINE_EXPR_VISIT(Select);
DEFINE_EXPR_VISIT(Call);
DEFINE_EXPR_VISIT(For);
DEFINE_EXPR_VISIT(Block);
DEFINE_EXPR_VISIT(IfThenElse);

}

} // namespace SC
