/*************************************************************************
	> File Name: irroamer.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Mar 2019 12:39:59 PM UTC
 ************************************************************************/

#ifndef _IRROAMER_H
#define _IRROAMER_H

#include "node.h"
#include "ir.h"

namespace
{

#define DEFINE_VISIT(type, node) \
    case NodeType::type: \
    return ((T*)this)->visit((const Node*)n, std::forward<Args>(args)...);

} // namespace

namespace SC
{

/*
 * \bref roam the node, it can be a visitor or a mutator.
 * refer to halide
 */
template <typename T, typename ExprRet, typename StmtRet>
class IRDispatcher
{
private:
    template <typename... Args>
    ExprRet dispatch_expr(const BaseExprNode* n, Args&&... args)
    {
        if(!n)
            return ExprRet{};
        switch (n->node_type)
        {
            // For base expr node, there are no stmt node_type
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
            /*case NodeType::LET_STMT:
            case NodeType::SELECT:
            case NodeType::FOR:
            case NodeType::BLOCK:
            case NodeType::STORE:
            case NodeType::REDUCE:
            case NodeType::IF_THEN_ELSE:*/
            default:
                CHECK_IF(0, "unsupported expr class");
        }
        return ExprRet{};
    }
    template <typename... Args>
    StmtRet dispatch_stmt(const BaseStmtNode* n, Args&&... args)
    {
        if(!n)
            return StmtRet{};
        switch (n->node_type)
        {
            DEFINE_VISIT(LET_STMT, LetStmt);
            DEFINE_VISIT(SELECT, Select);
            DEFINE_VISIT(FOR, For);
            DEFINE_VISIT(BLOCK, Block);
            DEFINE_VISIT(IF_THEN_ELSE, IfThenElse);
            DEFINE_VISIT(STORE, Store);
            DEFINE_VISIT(REDUCE, Reduce);
            DEFINE_VISIT(DMA_LOAD, DMALoad);
            DEFINE_VISIT(DMA_STORE, DMAStore);
            DEFINE_VISIT(ALLOCATE, Allocate);
            DEFINE_VISIT(FREE, Free);
            default:
                CHECK_IF(0, "unsupported stmt class");
        }
        return ExprRet{};
    }

public:
    template <typename... Args>
    inline StmtRet dispatch(const Stmt& stmt, Args&&... args)
    {
        return dispatch_stmt(stmt.get(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StmtRet dispatch(Stmt&& stmt, Args&&... args)
    {
        return dispatch_stmt(stmt.get(), std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    inline ExprRet dispatch(const Expr& expr, Args&&... args)
    {
        return dispatch_expr(expr.get(), std::forward<Args>(args)...);
    }
    template <typename... Args>
    inline ExprRet dispatch(Expr&& expr, Args&&... args)
    {
        return dispatch_expr(expr.get(), std::forward<Args>(args)...);
    }


};

} // namespace SC


#endif
