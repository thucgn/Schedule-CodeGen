/*************************************************************************
	> File Name: irmutator.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 01:32:57 PM UTC
 ************************************************************************/

#ifndef _IRMUTATOR_H
#define _IRMUTATOR_H

#include "node.h"
#include "ir.h"

namespace SC
{

/**
 * \bref IRMutator is used to traverse the IR and modified it
 */
class IRMutator
{
public:
    IRMutator() {}
    virtual ~IRMutator() {}

    virtual Expr mutate(const Expr& expr)
    {
        return expr.notNull() ? expr->mutate_expr(this) : nullptr;
    }

    virtual Stmt mutate(const Stmt& stmt)
    {
        return stmt.notNull() ? stmt->mutate_stmt(this) : nullptr;
    }

protected:
    
    virtual Expr visit(const IntImm* n);
    virtual Expr visit(const UIntImm* n);
    virtual Expr visit(const FloatImm* n);
    virtual Expr visit(const StringImm* n);
    virtual Expr visit(const Cast* n);
    virtual Expr visit(const Variable* n);
    virtual Expr visit(const Add* n);
    virtual Expr visit(const Sub* n);
    virtual Expr visit(const Mul* n);
    virtual Expr visit(const Div* n);
    virtual Expr visit(const Mod* n);
    virtual Expr visit(const Min* n);
    virtual Expr visit(const Max* n);
    virtual Expr visit(const Eq* n);
    virtual Expr visit(const Ne* n);
    virtual Expr visit(const Lt* n);
    virtual Expr visit(const Gt* n);
    virtual Expr visit(const Le* n);
    virtual Expr visit(const Ge* n);
    virtual Expr visit(const And* n);
    virtual Expr visit(const Or* n);
    virtual Expr visit(const Not* n);
    virtual Expr visit(const Call* n);
    virtual Stmt visit(const LetStmt* n);
    virtual Stmt visit(const Select* n);
    virtual Stmt visit(const For* n);
    virtual Stmt visit(const Block* n);
    virtual Stmt visit(const IfThenElse* n);

};

} // namespace SC

#endif
