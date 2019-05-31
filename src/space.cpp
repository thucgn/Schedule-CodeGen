/*************************************************************************
	> File Name: space.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 21 May 2019 06:56:39 AM UTC
 ************************************************************************/

#include "space.h"
#include "util.h"

namespace SC
{

/*void Space::define_split(Iter x, int min, int max)
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

void Space::define_split(Iter x, int min, int max)
{
    get()->splitspaces.emplace_back(SplitSpaceNode::make(std::move(x), min, max));
}*/


/*void Space::define_split(const std::string& tag, int min, int max)
{
    auto ret = get()->spaces.emplace(tag, SplitSpaceNode::make(Iter(), min, max));
    CHECK_IF(ret.second, "space tag %s exists", tag.c_str());
}
void Space::define_split(const std::string& tag, const std::vector<int>& candidates)
{
    auto ret = get()->spaces.emplace(tag, SplitSpaceNode::make(Iter(), candidates));
    CHECK_IF(ret.second, "space tag %s exists", tag.c_str());
}

void Space::define_reorder(const std::string& tag, const std::vector<std::vector<Iter>>& candidates)
{
    auto ret = get()->spaces.emplace(tag, ReorderSpaceNode::make(candidates));
    CHECK_IF(ret.second, "space tag %s exists", tag.c_str());
}
void Space::define_unroll(const std::string& tag, const std::vector<int>& candidates)
{
    auto ret = get()->spaces.emplace(tag, candidates);
    CHECK_IF(ret.second, "space tag %s exists", tag.c_str());
}*/


AxisPair Space::define_split(const std::string& tag, Axis axis,  int min, int max)
{
}
AxisPair Space::define_split(const std::string& tag, Axis axis, const std::vector<int>& candidates);
void Space::define_reorder(const std::string& tag, const std::vector< std::vector<Axis> >& candidates);
void Space::define_unroll(const std::string& tag, Axis axis, const std::vector<int>& candidates);
} // namespace SC
