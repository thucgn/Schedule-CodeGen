/*************************************************************************
	> File Name: iter.h
	> Author: 
	> Mail: 
	> Created Time: Mon 11 Mar 2019 07:46:15 AM UTC
 ************************************************************************/

#ifndef _ITER_H
#define _ITER_H

#include "node.h"
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
    IterType iter_type;
    IterSche iter_sche;
    Range range;
    VarExpr var;
    
    mutable RefCount ref_count;

    static Iter make(IterType iter_type, Range range, VarExpr var, IterSche iter_sche = IterSche::NO_SCHEDULE);
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

class Iter : public RefCountPtr<IterNode>
{
public:
    Iter() : RefCountPtr<IterNode>() {}
    explicit Iter(IterNode* p) : 
        RefCountPtr<IterNode>(p)
    {}

    

    /**
     * \bref return the var
     * note that: the convertion of iter to expr loses much infos.
     */
    operator Expr() const
    {
        return ptr->var;
    }
};


} // namespace SC

namespace std
{


} // namespace std

#endif
