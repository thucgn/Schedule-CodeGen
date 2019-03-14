/*************************************************************************
	> File Name: IR.h
	> Author: 
	> Mail: 
	> Created Time: Tue 05 Mar 2019 02:31:54 AM UTC
 ************************************************************************/

#ifndef _IR_H
#define _IR_H

#include "node.h"
#include <vector>
#include "function.h"

namespace SC
{

/**
 * \bref type of Call node, including external and internal call.
 */
enum class CallType : uint8_t
{
    EXTERNAL, // external function
    INTERNAL, // Function
};

/**
 * \bref type of for node
 */
enum class ForType : uint8_t
{
    SERIAL, //serial execution
    PARALLEL, // parallel execution
    VECTORIZED, // using vectorization intrinsics
    UNROLL // unroll execution
};

class Cast : public ExprNode<Cast>
{
public:
    Expr value;
    static const NodeType _node_type = NodeType::CAST;
    static Expr make(DataType t, Expr v);
};

template <typename T>
class BinaryOpNode : public ExprNode<T>
{
public:
    Expr a, b;
    static Expr make(Expr a, Expr b)
    {
        CHECK_IF(a.notNull(), "Expr a is null!");
        CHECK_IF(b.notNull(), "Expr a is null!");
        CHECK_IF(a.type() == b.type(), "type of Expr a, b can not match");
        const T* n = new T();
        n->data_type = a.type();
        n->a = std::move(a);
        n->b = std::move(b);
        return n;
    }
};

class Add : public BinaryOpNode<Add>
{
public:
    static const NodeType _node_type = NodeType::ADD;
};


class Div : public BinaryOpNode<Div>
{
public:
    static const NodeType _node_type = NodeType::DIV;
};

class Sub : public BinaryOpNode<Sub>
{
public:
    static const NodeType _node_type = NodeType::SUB;
};

class Mul : public BinaryOpNode<Mul>
{
public:
    static const NodeType _node_type = NodeType::MUL;
};

class Mod : public BinaryOpNode<Mod>
{
public:
    static const NodeType _node_type = NodeType::MOD;
};

class Min : public BinaryOpNode<Min>
{
public:
    static const NodeType _node_type = NodeType::MIN;
};

class Max : public BinaryOpNode<Max>
{
public:
    static const NodeType _node_type = NodeType::MAX;
};

template <typename T>
class CompareOpNode : public ExprNode<T>
{
public:
    Expr a, b;
    static Expr make(Expr a, Expr b)
    {
        CHECK_IF(a.notNull(), "Expr a is null!");
        CHECK_IF(b.notNull(), "Expr a is null!");
        CHECK_IF(a.type() == b.type(), "type of Expr a, b can not match");
        T* n = new T();
        n->data_type = Bool(a.type().length());
        n->a = std::move(a);
        n->b = std::move(b);
        return n;
    }
};

class Eq : public CompareOpNode<Eq>
{
public:
    static const NodeType _node_type = NodeType::EQ;
};

class Ne : public CompareOpNode<Ne>
{
public:
    static const NodeType _node_type = NodeType::NE;
};

class Lt : public CompareOpNode<Lt>
{
public:
    static const NodeType _node_type = NodeType::LT;
};

class Gt : public CompareOpNode<Gt>
{
public:
    static const NodeType _node_type = NodeType::GT;
};

class Le : public CompareOpNode<Le>
{
public:
    static const NodeType _node_type = NodeType::LE;
};

class Ge : public CompareOpNode<Ge>
{
public:
    static const NodeType _node_type = NodeType::GE;
};

class And : public ExprNode<And>
{
public:
    Expr a, b;
    static const NodeType _node_type = NodeType::AND;
    static Expr make(Expr a, Expr b);
};

class Or : public ExprNode<Or>
{
public:
    Expr a, b;
    static const NodeType _node_type = NodeType::OR;
    static Expr make(Expr a, Expr b);
};

class Not : public ExprNode<Not>
{
public:
    Expr value;
    static const NodeType _node_type = NodeType::NOT;
    static Expr make(Expr v);
};

/**
 * \bref variable class
 */
class Variable : public ExprNode<Variable>
{
public:
    std::string label; 
    static const NodeType _node_type = NodeType::VARIABLE;
    /**
     * \bref use address, not label, identifis the variable,
     */
    static Expr make(DataType type, const std::string& label);
};

VarExpr::VarExpr(DataType t, const std::string& label)
    : Expr(Variable::make(t, label))
{}

VarExpr::VarExpr(const std::string& label)
    : Expr(Variable::make(Int(32), label))
{}

VarExpr VarExpr::derive(const std::string& ext) const
{
    return VarExpr(get()->data_type, get()->label+ext);
}

Var Var::derive(const std::string& ext) const
{
    return Var(get()->data_type, get()->label+ext);
}


/**
 * \bref call function
 */
class Call : public ExprNode<Call>
{
public:
    using RetType = DataType;
    /**
     * \bref the name of the function
     */
    std::string name;
    /**
     * \bref args
     */
    std::vector<Expr> args;
    /**
     * \bref type of call, includeing external and internal calls
     */
    CallType call_type;
    /**
     * \bref function being called
     */
    Function func;

    static const NodeType _node_type = NodeType::CALL;

    static Expr make(RetType rt,
            CallType ct, 
            const std::string& name,
            std::vector<Expr>&& args,
            Function func);

};

/**
 * \bref Within the statement 'body', instances of var refer to value
 */
class LetStmt : public StmtNode<LetStmt>
{
public:
    VarExpr var;
    Expr value;
    Stmt body;
    static const NodeType _node_type = NodeType::LET_STMT;
    static Stmt make(VarExpr var, Expr value, Stmt body);
};

/**
 * \bref condition ? a : b;
 */
class Select : public StmtNode<Select>
{
public:
    Expr cond, true_case, false_case;
    static const NodeType _node_type = NodeType::SELECT;
    static Stmt make(Expr cond, Expr true_case, Expr false_case);
};

/**
 * \bref if cond then a else b
 */
class IfThenElse : public StmtNode<IfThenElse>
{
public:
    Expr cond;
    Stmt then_case, else_case;
    static const NodeType _node_type = NodeType::IF_THEN_ELSE;
    static Stmt make(Expr cond, Stmt then_case, Stmt else_case);
};

/**
 * \bref code block, composed of first stmt and the rest.
 */
class Block : public StmtNode<Block>
{
public:
    Stmt first, rest;
    static const NodeType _node_type = NodeType::BLOCK;
    static Stmt make(Stmt first, Stmt rest);
};

/**
 * \bref For
 */
class For : public StmtNode<For>
{
public:
    VarExpr var;
    Expr min, extent;
    Stmt body;
    ForType for_type;
    static const NodeType _node_type = NodeType::FOR;
    static Stmt make(ForType type, VarExpr var,
            Expr min, Expr extent,Stmt body);

};




} // SC

#endif
