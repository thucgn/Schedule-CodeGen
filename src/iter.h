/*************************************************************************
	> File Name: iter.h
	> Author: 
	> Mail: 
	> Created Time: Mon 11 Mar 2019 07:46:15 AM UTC
 ************************************************************************/

#ifndef _ITER_H
#define _ITER_H

#include "node.h"
#include "ir.h"
#include "refcountptr.h"

namespace SC
{

/**
 * \bref type of iter
 */
enum class IterType : uint8_t
{
    /**
     * \bref can be paralleled
     */
    PRARLLEL, 
    /**
     * \bref thread index, e.g. dim of CUDA
     * cannot: split, fuse, vectorized, parallel
     */
    THREAD_ID, 
    /**
     * \bref communicative reduction
     * cannot : parallel, vectorized
     */
    REDUCTION, 
    /**
     * \bref iterate with order
     * cannot : parallel, reorder, vectorized, 
     */
    ORDERED, 
    /**
     * \bref opaque iterator
     * cannot : all
     */
    OPAQUE,
};

/**
 * \bref schedule of iter
 */
enum class IterSche : uint8_t
{
    NO_SCHEDULE,
    /**
     * \bref indicates that it has been paralleled
     */
    PARALLELED, 
    /**
     * \bref indicates that it has been vectorized
     */
    VECTORIZED
};

struct Range
{
    Expr min, extent;
    Range():min(), extent() {}
    Range(Expr min, Expr extent) : 
        min(std::move(min)), extent(std::move(extent))
    {}
    bool is_defined() const 
    {
        return min.notNull() && extent.notNull();
    }
};

class Iter;

class IterNode
{
public:
    mutable IterType iter_type;
    mutable IterSche iter_sche;
    mutable Range range;
    //Var var;
    
    mutable RefCount ref_count;

    static Iter make(IterType iter_type, Range range, Var var, IterSche iter_sche = IterSche::NO_SCHEDULE);
};

template <>
inline RefCount& ref_count<IterNode>(const IterNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy<IterNode>(const IterNode* p)
{
    if(p)
        delete p;
}

class Iter : public RefCountPtr<const IterNode>
{
public:
    Iter() : RefCountPtr<const IterNode>() {}
    explicit Iter(const IterNode* p) : 
        RefCountPtr<const IterNode>(p)
    {}

    operator Expr() const
    {
        
    }
};

Iter IterNode::make(IterType iter_type, Range range, 
        Var var, IterSche iter_sche)
{
    CHECK_IF(var.notNull(), "var is null");
    IterNode* n = new IterNode();
    n->iter_type = iter_type;
    n->range = range;
    n->var = std::move(var);
    n->iter_sche = iter_sche;
    return n;
}

} // namespace SC


#endif
