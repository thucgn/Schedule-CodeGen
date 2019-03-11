/*************************************************************************
	> File Name: node.h
	> Author: 
	> Mail: 
	> Created Time: Tue 05 Mar 2019 02:34:21 AM UTC
 ************************************************************************/

#ifndef _NODE_H
#define _NODE_H

#include <string>
#include "refcountptr.h"
#include "type.h"
#include "util.h"

namespace SC
{

class IRVisitor;
class IRMutator;
class Variable;

class NodeRef;
class Stmt;
class Expr;

enum class NodeType : uint8_t
{
    INTIMM,
    UINTIMM,
    FLOATIMM,
    STRINGIMM,
    CAST,
    VARIABLE,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    MIN,
    MAX,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    AND,
    OR,
    NOT,
    CALL,
    //LET,
    LET_STMT,
    SELECT,
    FOR,
    BLOCK,
    IF_THEN_ELSE,
};


/**
 * \bref basic node, parent of IR node
 */
class Node
{
public:

    virtual ~Node() {}

    Node(NodeType type) : node_type(type) {}

    /**
     * \bref visitor pattern, a visitor can traverse of mutate the memebers of IRNode
     * \param v, pointer of IRVisitor
     */
    virtual void accept(IRVisitor *v) const = 0;

    // reference counter, managed by NodeRef
    mutable RefCount ref_count;
    // node type
    NodeType node_type;

};

template <> inline RefCount& ref_count<Node>(const Node* p) 
{
    return p->ref_count;
}
template <> inline void destroy<Node>(const Node* p)
{
    if(p) delete p;
}

/**
 * \bref Node Pointer class with reference count.
 */
class NodeRef : public RefCountPtr<const Node>
{
public:    
    NodeRef() : RefCountPtr<const Node>() {}
    NodeRef(const Node* p) : RefCountPtr<const Node>(p) {}

    /**
     * \bref try cast the ptr to const T*
     * \return if cast succeed, return ptr, or nullptr
     */
    template <typename T>
    const T* cast_to() const 
    {
        if(ptr)
        {
            if(is_type<T>())
                return (const T*)ptr;
        }
        return nullptr;
    }

    /*
     * \bref check the type of node
     */
    template <typename T>
    bool is_type() const 
    { 
        return (ptr->node_type == T::_node_type);
    }

    NodeType node_type() const
    {
        return ptr->node_type;
    }

    /*
     * \bref dispatch visitor
     * \param v visitor
     */
    void accept(IRVisitor* v) const
    {
        ptr->accept(v);
    }
};


/*
 * \bref base statement node, all statment node inherit the class
 */
class BaseStmtNode : public Node
{
public:
    BaseStmtNode(NodeType type) : Node(type) {}
    virtual Stmt mutate_stmt(IRMutator* v) const = 0;
};

/*
 * \bref base expression node, all expression node inherit the class
 */
class BaseExprNode : public Node
{
public:
    BaseExprNode(NodeType type) : Node(type) {}
    virtual Expr mutate_expr(IRMutator* v) const = 0;

    // data type, e.g. type of int
    DataType data_type;
};


/*
 * \bref template class of ExprNode, which is used to dispatch functions
 */
template <typename T>
class ExprNode : public BaseExprNode
{
public:
    ExprNode() : BaseExprNode(T::_node_type){}
    virtual ~ExprNode() {}

    void accept(IRVisitor* v) const override;
    Expr mutate_expr(IRMutator* v) const override;
};

/*
 * \bref template class of ExprNode, which is used to dispatch functions
 */
template <typename T>
class StmtNode : public BaseStmtNode
{
public:
    StmtNode() : BaseStmtNode(T::_node_type){}
    virtual ~StmtNode() {}

    void accept(IRVisitor* v) const override;
    Stmt mutate_stmt(IRMutator* v) const override;
};

class IntImm : public ExprNode<IntImm>
{
public:
    int64_t value;
    static const NodeType _node_type = NodeType::INTIMM;
    static const IntImm* make(DataType t, int64_t v)
    {
        CHECK_IF(t.isInt() && t.isScalar(), "Not a scalar Int, or unsupported bits");
        v = (int64_t)(((uint64_t)v) << (64-t.bits())); //no-sign shift
        v = (v >> (64-t.bits())); // sign shift
        IntImm* n = new IntImm();
        n->value = v;
        n->data_type = t;
        return n;
    }
};

class UIntImm : public ExprNode<UIntImm>
{
public: 
    uint64_t value;
    static const NodeType _node_type = NodeType::UINTIMM;
    static const UIntImm* make(DataType t, uint64_t v)
    {
        CHECK_IF(t.isUint() && t.isScalar(), "Not a scalar UInt, or unsupported bits");
        v <<= (64-t.bits());
        v >>= (64-t.bits());
        UIntImm* n = new UIntImm();
        n->value = v;
        n->data_type = t;
        return n;
    }
};

class FloatImm : public ExprNode<FloatImm>
{
public:
    double value;
    static const NodeType _node_type = NodeType::FLOATIMM;
    static const FloatImm* make(DataType t, double v)
    {
        CHECK_IF(t.isUint() && t.isScalar(), "Not a scalar UInt, or unsupported bits");
        FloatImm* n = new FloatImm();
        if(t.bits() == 32)
            v = (float)v;
        n->value = v;
        n->data_type = t;
        return n;
    }
};

class StringImm : public ExprNode<StringImm>
{
public:
    std::string value;
    static const NodeType _node_type = NodeType::STRINGIMM;
    static const StringImm* make(const std::string& v)
    {
        StringImm* n = new StringImm();
        n->value = v;
        return n;
    }
};

class Stmt : public NodeRef
{
public:
    Stmt() : NodeRef() {}
    Stmt(const BaseStmtNode* node) : NodeRef(node) {}

    /*
     * \bref get pointer of Base Stmt Node
     * the original get() of NodeRef returns const Node*
     */
    const BaseStmtNode* get() const 
    {
        return (const BaseStmtNode*)ptr;
    }

    const BaseStmtNode& operator*() const
    {
        return *((const BaseStmtNode*)ptr);
    }

    const BaseStmtNode* operator->() const
    {
        return (const BaseStmtNode*)ptr;
    }


};


class Expr : public NodeRef
{
public:
    Expr() : NodeRef() {}
    Expr(const BaseExprNode* node) : NodeRef(node){}
    
    explicit Expr(int8_t v) : 
        NodeRef(IntImm::make(Int(8), v))
    {}
    explicit Expr(int16_t v) : 
        NodeRef(IntImm::make(Int(16), v))
    {}
    explicit Expr(int32_t v) : 
        NodeRef(IntImm::make(Int(32), v))
    {}
    explicit Expr(int64_t v) : 
        NodeRef(IntImm::make(Int(64), v))
    {}
    explicit Expr(uint8_t v) : 
        NodeRef(IntImm::make(UInt(8), v))
    {}
    explicit Expr(uint16_t v) : 
        NodeRef(IntImm::make(UInt(16), v))
    {}
    explicit Expr(uint32_t v) : 
        NodeRef(IntImm::make(UInt(32), v))
    {}
    explicit Expr(uint64_t v) : 
        NodeRef(IntImm::make(UInt(64), v))
    {}
    explicit Expr(float v) :
        NodeRef(FloatImm::make(Float(32), v))
    {}
    explicit Expr(double v) :
        NodeRef(FloatImm::make(Float(64), v))
    {}
    Expr(const std::string& v) :
        NodeRef(StringImm::make(v))
    {}

    const DataType type() const { return get()->data_type; }

    const BaseExprNode* get() const 
    { 
        return (const BaseExprNode*)ptr; 
    }

    const BaseExprNode& operator*() const
    {
        return *((const BaseExprNode*)ptr);
    }

    const BaseExprNode* operator->() const 
    {
        return (const BaseExprNode*)ptr;
    }
};

/**
 * \bref varexpr only refer to Variable of IR
 */
class VarExpr : public Expr
{
public:
    VarExpr() : Expr() {}
    explicit VarExpr(const BaseExprNode* node) : Expr(node) {}

    //implemented in ir.h, because Variable is defined there.
    VarExpr(DataType t, const std::string& label);
    VarExpr(const std::string& lable);

    const Variable* get() const 
    { 
        return (const Variable*)ptr; 
    }
};


/*
 * \bref stmt compare class
 * compare the value of pointer, i.e. address
 */
class StmtCompare
{
public:
    bool operator()(const Stmt& o1, const Stmt& o2) const
    {
        return (o1.get() < o2.get());
    }
};

/*
 * \bref expr compare class
 * compare the value of pointer, i.e. address
 */
class ExprCompare
{
public:
    bool operator()(const Expr& o1, const Expr& o2) const
    {
        return (o1.get() < o2.get());
    }
};

} // namespace SC

#endif
