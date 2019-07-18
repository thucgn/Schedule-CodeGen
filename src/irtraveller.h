/*************************************************************************
	> File Name: irtraveller.h
	> Author: 
	> Mail: 
	> Created Time: Thu 18 Jul 2019 07:35:38 AM UTC
 ************************************************************************/

#ifndef _IRTRAVELLER_H
#define _IRTRAVELLER_H

#include "node.h"
#include "ir.h"

namespace 
{
#define DEFINE_VISIT(type, node) \
    case NodeType::type: \
        visit(n.cast_to<node>()); \
        break;
} // namespace

namespace SC
{

/**
 * \bref traverse the IR
 */
class IRTraveller
{
public:
    virtual void traverse(const NodeRef& n)
    {
        if(!n.notNull())
           return; 

        switch(n->node_type)
        {
            DEFINE_VISIT(INTIMM, IntImm);
            DEFINE_VISIT(UINTIMM, UIntImm);
            DEFINE_VISIT(FLOATIMM, FloatImm);
            DEFINE_VISIT(STRINGIMM, StringImm);
            DEFINE_VISIT(CAST, Cast);
            DEFINE_VISIT(VARIABLE, Variable);
            DEFINE_VISIT(ADD, Add);
            DEFINE_VISIT(SUB, Sub);
            DEFINE_VISIT(MUL, Mul);
            DEFINE_VISIT(DIV, Div);
            DEFINE_VISIT(MOD, Mod);
            DEFINE_VISIT(MIN, Min);
            DEFINE_VISIT(MAX, Max);
            DEFINE_VISIT(EQ, Eq);
            DEFINE_VISIT(NE, Ne);
            DEFINE_VISIT(LT, Lt);
            DEFINE_VISIT(GT, Gt);
            DEFINE_VISIT(LE, Le);
            DEFINE_VISIT(GE, Ge);
            DEFINE_VISIT(AND, And);
            DEFINE_VISIT(OR, Or);
            DEFINE_VISIT(NOT, Not);
            DEFINE_VISIT(CALL, Call);
            DEFINE_VISIT(LET_STMT, LetStmt);
            DEFINE_VISIT(SELECT, Select);
            DEFINE_VISIT(FOR, For);
            DEFINE_VISIT(BLOCK, Block);
            DEFINE_VISIT(IF_THEN_ELSE, IfThenElse);
            DEFINE_VISIT(STORE, Store);
            DEFINE_VISIT(REDUCE, Reduce);
            DEFINE_VISIT(EVALUEATE, Evaluate);
            DEFINE_VISIT(DMA_LOAD, DMALoad);
            DEFINE_VISIT(DMA_STORE, DMAStore);
            DEFINE_VISIT(ALLOCATE, Allocate);
            DEFINE_VISIT(FREE, Free);
        }
    }
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
    virtual void visit(const Store* n);
    virtual void visit(const Reduce* n);
    virtual void visit(const Evaluate* n);
    virtual void visit(const DMALoad* n);
    virtual void visit(const DMAStore* n);
    virtual void visit(const Allocate* n);
    virtual void visit(const Free* n);
};

} // namespace SC

#endif
