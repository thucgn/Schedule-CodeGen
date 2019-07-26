/*************************************************************************
	> File Name: schedule.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 13 Mar 2019 03:15:33 AM UTC
 ************************************************************************/

#include "schedule.h"
#include <unordered_set>
#include <cstdlib>
#include <algorithm>
#include "util.h"
#include "hash.h"
#include "node.h"
#include "ir.h"
#include "iroperator.h"

namespace 
{

/**
 * \bref compare two Iter by comparing the internal pointer
 */
bool iterEqual(const SC::Iter& x1, const SC::Iter& x2)
{
    return x1.get() == x2.get();
}

/**
 * \bref find position of Iter
 * \return the position in vector, or size of vec
 */
size_t findIter(const std::vector<SC::Iter>& vec, 
        const SC::Iter& x)
{
    size_t i = 0;
    for(;i < vec.size(); i ++)
    {
        if(iterEqual(vec[i], x))
            break;
    }
    return i;
}


} // namespace

namespace SC
{

Stage StageNode::make(Computation cp)
{
    CHECK_IF(cp.notNull(), "computation is null");
    StageNode* s = new StageNode();
    s->all_iters = cp->rootIters();
    s->root_iters = cp->rootIters();
    s->original_cp = cp;
    s->scheduled_cp = std::move(cp);
    return Stage(s);
}

Stage& Stage::fuse(Iter x1, Iter x2, Iter& target)
{
    CHECK_IF(x1.notNull() && x2.notNull(), "iters to be fused are null");
    CHECK_IF(x1->iter_type == IterType::PRARLLEL
            || x1->iter_type == IterType::REDUCTION
            || x1->iter_type == IterType::ORDERED,
            "itertype of %s : %s cannot be fused", 
            x1->var->label.c_str(),
            itertype2str(x1->iter_type).c_str());
    CHECK_IF(x2->iter_type == IterType::PRARLLEL
            || x2->iter_type == IterType::REDUCTION
            || x2->iter_type == IterType::ORDERED,
            "itertype of %s : %s cannot be fused", 
            x2->var->label.c_str(),
            itertype2str(x2->iter_type).c_str());

    auto& root_iters = get()->root_iters;
    auto& all_iters = get()->all_iters;

    size_t pos1 = findIter(root_iters, x1);
    size_t pos2 = findIter(root_iters, x2);


    if(pos1 >= root_iters.size()) // not found
    {
        if(findIter(all_iters, x1) < all_iters.size())
            CHECK_IF(false, "iter %s has been fused or splitted", x1->var->label.c_str());
        else
            CHECK_IF(false, "there is no this iter : %s", x1->var->label.c_str());
    }
    if(pos2 >= root_iters.size()) // not found
    {
        if(findIter(all_iters, x2) < all_iters.size())
            CHECK_IF(false, "iter %s has been fused or splitted", x2->var->label.c_str());
        else
            CHECK_IF(false, "there is no this iter : %s", x2->var->label.c_str());
    }

    size_t pos_min = pos1, pos_max = pos2;
    if(pos_min > pos_max)
        std::swap(pos_min, pos_max);
    CHECK_IF(pos_min+1 == pos_max, "the iters to be fused is not contiguous");

    // the range of target starts from 0, and the real min is set by apply fuse result
    Range target_range = {0, x1->range.extent * x2->range.extent};
    Iter target_iter = IterNode::make(x1->iter_type, target_range, 
            x1->var.derive(std::string("_FUSE_") + x2->var->label),
            x1->iter_sche);
    target = target_iter;

    get()->fuse_results.emplace_back(FuseResult(x1, x2, target_iter));
    
    //substitude x1,x2 in root_iters with target
    root_iters.erase(root_iters.begin() + pos_max);
    root_iters.erase(root_iters.begin() + pos_min);
    root_iters.insert(root_iters.begin()+pos_min, target);


    all_iters.emplace_back(std::move(target_iter));

    return *this;
}


Stage& Stage::split(Iter x, Iter& outer_ref, Iter& inner_ref, Expr factor)
{
    CHECK_IF(x.notNull(), "iter to be splitted is null");
    //note that factor should be greater than 0
    CHECK_IF(factor.notNull(), "split factor is null");
    CHECK_IF(x->iter_type == IterType::PRARLLEL
            || x->iter_type == IterType::REDUCTION
            || x->iter_type == IterType::ORDERED, 
            " cannot split the iter type %s", itertype2str(x->iter_type).c_str());
    
    Range outer_range{x->range.min, (x->range.extent+factor-1)/factor};
    Range inner_range{0, factor};
    Iter outer = IterNode::make( x->iter_type, outer_range, 
            x->var.derive("_o"),
            x->iter_sche);
    Iter inner = IterNode::make( x->iter_type, inner_range,
            x->var.derive("_i"),
            x->iter_sche);
    //set the reference
    outer_ref = outer;
    inner_ref = inner;

    size_t pos = findIter(get()->root_iters, x);
    if(pos >= get()->root_iters.size()) // not found
    {
        pos = findIter(get()->all_iters, x);
        if(pos < get()->all_iters.size())
        {
            LOG("iter %s has been splitted", x->var->label.c_str());
            CHECK_IF(false, "iter to be splitted has been splited");
        }
        else
        {
            LOG("iter %s has been splitted", x->var->label.c_str());
            CHECK_IF(false, "cannot find the iter");
        }
    }

    //LOG("position of %s : %d", x->var->label.c_str(), (int)pos);
    // record split
    get()->split_results.emplace_back(SplitResult(x, outer, inner, factor));

    auto& root_iters = get()->root_iters;
    auto& all_iters = get()->all_iters;

    // subsitude x in root_iters with outer and inner
    root_iters.erase(root_iters.begin() + pos);
    root_iters.insert(root_iters.begin() + pos, outer);
    root_iters.insert(root_iters.begin() + pos + 1, inner);
    // add outer and inner to all iters
    all_iters.emplace_back(std::move(outer));
    all_iters.emplace_back(std::move(inner));

    return *this;
}

Stage& Stage::reorder(const std::vector<Iter>& ordered_iters)
{
    CHECK_IF(ordered_iters.size() == get()->root_iters.size(), "cannot reorder partial iters");
    // verify the iters    
    std::unordered_set<Iter> count_iters;
    for(auto& iter : ordered_iters)
    {
        CHECK_IF(iter->iter_type == IterType::PRARLLEL 
                || iter->iter_type == IterType::REDUCTION
                || iter->iter_type == IterType::THREAD_ID,
                "itertype of %s : %s cannot be reordered", 
                iter->var->label.c_str(), 
                itertype2str(iter->iter_type).c_str());
        CHECK_IF(count_iters.count(iter) == 0, "iter:%s appear more than once",
                iter->var->label.c_str());
        count_iters.insert(iter);
    }
    
    auto& root_iters = get()->root_iters;
    std::vector<int> poss;
    for(auto& iter : ordered_iters)
    {
        size_t pos = findIter(root_iters, iter);
        CHECK_IF(pos < root_iters.size(), "cannot find the iter:%s", iter->var->label.c_str());
        poss.push_back(pos);
    }
    // store the ordered iter
    std::vector<Iter> tmp;
    for(int i : poss)
        tmp.emplace_back(std::move(root_iters[i]));
    // reassign root_iters
    root_iters.clear();
    for(auto& iter : tmp)
       root_iters.emplace_back(std::move(iter));

    return *this;
}

Stage& Stage::parallel(const Iter iter)
{
    size_t pos = findIter(get()->root_iters, iter);
    CHECK_IF(pos < get()->root_iters.size(), "cannot find the parallel iter:%s", iter->var->label.c_str());
    Iter& target_iter = get()->root_iters[pos];
    CHECK_IF(target_iter->iter_type == IterType::PRARLLEL, "cannot parallel this loop, because the itertype cannot be paralleld");
    get()->root_iters[pos]->iter_sche = IterSche::PARALLELED;
    return *this;
}

Stage& Stage::vectorization(const Iter iter)
{
    size_t pos = findIter(get()->root_iters, iter);
    CHECK_IF(pos < get()->root_iters.size(), "cannot find the vectorization iter:%s", iter->var->label.c_str());
    Iter& target_iter = get()->root_iters[pos];
    CHECK_IF(target_iter->iter_type == IterType::PRARLLEL, "cannot vectorized this loop, because the itertype cannot be vectorized");
    return *this;
}

Stage& Stage::unroll(const Iter iter)
{
    CHECK_IF(false, "unroll has not been implemented");
}

Stage& Stage::compute_inline()
{
    get()->attach_type = AttachType::INLINE;
    return *this;
}

Stage& Stage::compute_root()
{
    get()->attach_type = AttachType::ROOT;
    return *this;
}

Stage& Stage::compute_at(Stage s, Iter scope)
{
    bool found = false;
    for(auto& iter : s->root_iters)
        if(iter.sameAs(scope))
            found = true;
    CHECK_IF(found, "no this axis in stage");

    get()->attach_type = AttachType::SCOPE;
    get()->attach_iter = std::move(scope);
    get()->attach_stage = std::move(s);

    return *this;
}

Schedule ScheduleNode::make(Computation cp)
{
    CHECK_IF(cp.notNull(), " initial computation of schedule cannot be NULL");
    CHECK_IF(false, "not implemented");
    return ScheduleNode::make();
}

Schedule ScheduleNode::make(std::vector<Computation> cps)
{
    CHECK_IF(cps.size() > 0, "computation vector is empty");
    ScheduleNode* s = new ScheduleNode();
    for(auto& cp : cps)
    {
        s->stages.emplace_back(
                StageNode::make(std::move(cp))); 
    }
    return Schedule(s);
}


Schedule ScheduleNode::make(std::vector<Stage> ss)
{
    CHECK_IF(ss.size() > 0, "stages vector is empty");
    ScheduleNode* s = new ScheduleNode();
    s->stages = std::move(ss);
    return Schedule(s);
}

Schedule ScheduleNode::make()
{
    return Schedule(new ScheduleNode());
}

Stage Schedule::addComputation(Computation cp)
{
    CHECK_IF(get()->cp2stage.count(cp) == 0, "Dumplicated cp");
    Stage s = StageNode::make(cp);
    get()->stages.push_back(s);
    get()->cp2stage.emplace(cp, s);
    return s;
}

} // namespace SC
