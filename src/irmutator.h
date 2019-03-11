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
    
    virtual Expr mutateNode(const IntImm* n);
    virtual Expr mutateNode(const UIntImm* n);
    virtual Expr mutateNode(const FloatImm* n);
    virtual Expr mutateNode(const StringImm* n);
    virtual Expr mutateNode(const Cast* n);
    virtual Expr mutateNode(const Variable* n);
    virtual Expr mutateNode(const Add* n);
    virtual Expr mutateNode(const Sub* n);
    virtual Expr mutateNode(const Mul* n);
    virtual Expr mutateNode(const Div* n);
    virtual Expr mutateNode(const Mod* n);
    virtual Expr mutateNode(const Min* n);
    virtual Expr mutateNode(const Max* n);
    virtual Expr mutateNode(const Eq* n);
    virtual Expr mutateNode(const Ne* n);
    virtual Expr mutateNode(const Lt* n);
    virtual Expr mutateNode(const Gt* n);
    virtual Expr mutateNode(const Le* n);
    virtual Expr mutateNode(const Ge* n);
    virtual Expr mutateNode(const And* n);
    virtual Expr mutateNode(const Or* n);
    virtual Expr mutateNode(const Not* n);
    virtual Expr mutateNode(const Call* n);
    virtual Stmt mutateNode(const LetStmt* n);
    virtual Stmt mutateNode(const Select* n);
    virtual Stmt mutateNode(const For* n);
    virtual Stmt mutateNode(const Block* n);
    virtual Stmt mutateNode(const IfThenElse* n);

    template <typename T>
    friend class ExprNode;
    template <typename T>
    friend class StmtNode;

};

} // namespace SC

#endif
