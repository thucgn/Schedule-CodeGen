/*************************************************************************
	> File Name: Function.h
	> Author: 
	> Mail: 
	> Created Time: Wed 06 Mar 2019 01:03:35 PM UTC
 ************************************************************************/

#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "refcountptr.h"

namespace SC
{

enum class FunctionNodeType : uint8_t
{
    NEST_LOOP
};

class FunctionNode
{
public:
    std::string name;
    const std::string& getName() const { return name; }

    mutable RefCount ref_count;
    FunctionNodeType node_type;

    FunctionNode(FunctionNodeType type) : node_type(type) {}

    virtual ~FunctionNode() {}
};

template <>
inline RefCount& ref_count<FunctionNode>(const FunctionNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy<FunctionNode>(const FunctionNode* p)
{
    if(p)
        delete p;
}

class Function : public RefCountPtr<const FunctionNode>
{
public: 

    Function():RefCountPtr<const FunctionNode>() {}
    explicit Function(const FunctionNode* p) : 
        RefCountPtr<const FunctionNode>(p)
    {}


    /*const FunctionNode* get() const { return (const FunctionNode*)ptr; }
    const FunctionNode* operator->() const { return get(); }
    const FunctionNode& operator*() const 
    {
        return *(get());
    }*/

    template <typename T>
    bool is_type() const { return ptr->node_type == T::_node_type; }

    template <typename T>
    const T* cast_to() const {
        return (const T*)ptr;
    }
    FunctionNodeType node_type() const { return ptr->node_type; }
};

} // namespace SC

#endif
