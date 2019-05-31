/*************************************************************************
	> File Name: axis.h
	> Author: 
	> Mail: 
	> Created Time: Thu 30 May 2019 09:07:08 AM UTC
 ************************************************************************/

#ifndef _AXIS_H
#define _AXIS_H

#include "refcountptr.h"
#include "iter.h"

namespace SC
{

struct VirtualAxis;

struct VirtualAxisNode
{
    Iter x;
    mutable RefCount ref_count;
    static VirtualAxis make(Iter x);
};

template <>
inline RefCount& ref_count<VirtualAxisNode>(const VirtualAxisNode* p) {
    return p->ref_count;
}

template <>
inline void destroy<VirtualAxisNode>(const VirtualAxisNode* p) {
    if(p)
        delete p;
}

struct VirtualAxis : public RefCountPtr<VirtualAxisNode>
{
    VirtualAxis(Iter x) :
        RefCountPtr<VirtualAxisNode>( VirtualAxisNode::make(x))
    {}
    VirtualAxis(VirtualAxisNode* p):
        RefCountPtr<VirtualAxisNode>(p)
    {}
    VirtualAxis() : 
        RefCountPtr<VirtualAxisNode>()
    {}

};

typedef VirtualAxis Axis;
typedef VirtualAxisNode AxisNode;
using AxisPair = std::pair<Axis, Axis>;


} // namespace SC

#endif
