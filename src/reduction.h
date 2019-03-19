/*************************************************************************
	> File Name: reduction.h
	> Author: 
	> Mail: 
	> Created Time: Mon 18 Mar 2019 01:52:42 PM UTC
 ************************************************************************/

#ifndef _REDUCTION_H
#define _REDUCTION_H

#include <vector>
#include "node.h"

namespace SC
{

class CommReducer;
/**
 * \bref refer to tvm. Reducer
 */
class CommReducerNode
{
public:

    std::vector<Var> lhs;
    std::vector<Var> rhs;
    /**
     *\bref result of reduce
     */
    std::vector<Expr> result;

    std::vector<Expr> identity_element;

    /**
     * \bref call to combine a and b
     */
    std::vector<Expr> operator()(
            std::vector<Expr> a,
            std::vector<Expr> b) const;
    
    static CommReducer make(
            std::vector<Var> lhs,
            std::vector<Var> rhs,
            std::vector<Expr> result,
            std::vector<Expr> identity_element);

    mutable RefCount ref_count;
};

template <>
inline RefCount& ref_count<const CommReducerNode>(
        const CommReducerNode* p)
{
    return  p->ref_count;
}

template <>
inline void destroy<const CommReducerNode>(
        const CommReducerNode* p)
{
    if(p)
        delete p;
}

class CommReducer : public RefCountPtr<const CommReducerNode> 
{
public:
    CommReducer() : RefCountPtr<const CommReducerNode>() {}
    CommReducer(const CommReducerNode* p) :
        RefCountPtr<const CommReducerNode>(p) 
    {}
};

} // namespace SC

#endif
