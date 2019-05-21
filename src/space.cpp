/*************************************************************************
	> File Name: space.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 21 May 2019 06:56:39 AM UTC
 ************************************************************************/

#include "space.h"

namespace SC
{

void Space::define_split(Iter x, int min, int max)
{
    get()->splitspaces.emplace_back(SplitSpaceNode::make(std::move(x), min, max));
}

void Space::define_split(Iter x, const std::vector<int>& candidates)
{
    get()->splitspaces.emplace_back(
            SplitSpaceNode::make(std::move(x), 
                std::forward<const std::vector<int>&>(candidates)));
}

void Space::define_reorder(const std::vector<std::vector<Iter>>& candidates)
{
    get()->reorderspaces.emplace_back(candidates);
}

void Space::define_unroll(Iter x, const std::vector<int>& candidates)
{
    get()->unrollspaces.emplace_back(std::move(x), candidates);
}

} // namespace SC
