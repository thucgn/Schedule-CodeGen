/*************************************************************************
	> File Name: tensor_base.h
	> Author: 
	> Mail: 
	> Created Time: Mon 18 Mar 2019 08:51:53 AM UTC
 ************************************************************************/

#ifndef _TENSOR_BASE_H
#define _TENSOR_BASE_H

#include <string>
#include <vector>
#include "refcountptr.h"
#include "node.h"
#include "function.h"

namespace SC
{

class TensorBaseNode
{
public:
    std::string name;
    std::vector<Expr> shape;

    mutable RefCount ref_count;

    virtual ~TensorBaseNode() {}
};

template <>
inline RefCount& ref_count<TensorBaseNode>(
        const TensorBaseNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy<TensorBaseNode>(
        const TensorBaseNode* p)
{
    if(p)
        delete p;
}


class TensorBase : public RefCountPtr<const TensorBaseNode>
{
public:
    TensorBase() : RefCountPtr<const TensorBaseNode>() {}
    explicit TensorBase(const TensorBaseNode* p) :
        RefCountPtr<const TensorBaseNode>(p)
    {}

    virtual const std::vector<Expr>& shape() const {
        return get()->shape;
    }
    const std::string& name() const { return ptr->name; }

    template <typename T>
    const T* cast_to() const {
        static_assert(std::is_base_of<TensorBaseNode, T>::value, "cannot convert to TensorBase");
        return (const T*)ptr;
    }
};

} // namespace SC


#endif
