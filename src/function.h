/*************************************************************************
	> File Name: Function.h
	> Author: 
	> Mail: 
	> Created Time: Wed 06 Mar 2019 01:03:35 PM UTC
 ************************************************************************/

#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "node.h"

namespace SC
{

class FunctionNode : public Node
{
public:
    std::string name;
    const std::string& getName() const { return name; }
};

class Function : public NodeRef
{
public: 
    const FunctionNode* get() const { return (const FunctionNode*)ptr; }
    const FunctionNode* operator->() const { return get(); }
    const FunctionNode& operator*() const 
    {
        return *(get());
    }
};

} // namespace SC

#endif
