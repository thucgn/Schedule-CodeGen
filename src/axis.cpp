/*************************************************************************
	> File Name: axis.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Jun 2019 06:42:47 AM UTC
 ************************************************************************/

#include "axis.h"

namespace SC
{

VirtualAxis VirtualAxisNode::make(Iter x) {
    VirtualAxisNode* n = new VirtualAxisNode;
    n->x = std::move(x);
    return n;
}

} // namespace SC
