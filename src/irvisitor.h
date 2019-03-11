/*************************************************************************
	> File Name: irvisitor.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 05:50:19 AM UTC
 ************************************************************************/

#ifndef _IRVISITOR_H
#define _IRVISITOR_H

#include "node.h"
#include "ir.h"

namespace SC
{

/**
 * \bref visitor pattern
 * visit recursively the AST
 */
class IRVisitor
{

public:
    virtual void visit(const IntImm* n);
    virtual void visit(const UIntImm* n);
    virtual void visit(const FloatImm* n);
    virtual void visit(const StringImm* n);
    virtual void visit(const Cast* n);
    virtual void visit(const Variable* n);
    virtual void visit(const Add* n);
    virtual void visit(const Sub* n);
    virtual void visit(const Mul* n);
    virtual void visit(const Div* n);
    virtual void visit(const Mod* n);
    virtual void visit(const Min* n);
    virtual void visit(const Max* n);
    virtual void visit(const Eq* n);
    virtual void visit(const Ne* n);
    virtual void visit(const Lt* n);
    virtual void visit(const Gt* n);
    virtual void visit(const Le* n);
    virtual void visit(const Ge* n);
    virtual void visit(const And* n);
    virtual void visit(const Or* n);
    virtual void visit(const Not* n);
    virtual void visit(const LetStmt* n);
    virtual void visit(const Select* n);
    virtual void visit(const Call* n);
    virtual void visit(const For* n);
    virtual void visit(const Block* n);
    virtual void visit(const IfThenElse* n);

};

} // namespace SC

#endif
