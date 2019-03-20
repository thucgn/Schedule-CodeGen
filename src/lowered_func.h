/*************************************************************************
	> File Name: lowered_func.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 08:20:27 AM UTC
 ************************************************************************/

#ifndef _LOWERED_FUNC_H
#define _LOWERED_FUNC_H

#include "schedule.h"
#include "refcountptr.h"

namespace SC
{


class LoweredFuncNode
{
public:
    Stmt body;
    mutable RefCount ref_count;
};

template <>
inline RefCount& ref_count(const LoweredFuncNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy(const LoweredFuncNode* p)
{
    if(p)
        delete p;
}

class LoweredFunc : public RefCountPtr<const LoweredFuncNode>
{
public:
    LoweredFunc() : RefCountPtr() {}
    explicit LoweredFunc(const LoweredFuncNode* p) :
        RefCountPtr(p)
    {}
    const LoweredFuncNode* get() const { return static_cast<const LoweredFuncNode*>(ptr); }
    const LoweredFuncNode* operator->() const { return get(); }
    const LoweredFuncNode& operator*() const { return *get(); }

};


LoweredFunc lower(Schedule s);

} // namespace SC


#endif
