/*************************************************************************
	> File Name: schedule.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 11:00:43 AM UTC
 ************************************************************************/

#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "refcountptr.h"
#include "computation.h"
#include "hash.h"
#include "ir.h"

namespace SC
{

class Stage;
class Schedule;

struct SplitResult
{
    Iter x;
    Iter outer;
    Iter inner;
    Expr factor;
    SplitResult(Iter x_, Iter outer_, Iter inner_, Expr factor_) : 
        x(std::move(x_)),  outer(std::move(outer_)), 
        inner(std::move(inner_)), factor(std::move(factor_))
    {}
};

struct FuseResult
{
    Iter x1;
    Iter x2;
    Iter target;
    FuseResult(Iter x1_, Iter x2_, Iter target_):
        x1(std::move(x1_)), x2(std::move(x2_)), target(std::move(target_))
    {}
};


/**
 * \bref a stage is corresponding to a computation
 */
class StageNode final
{
public:

    /**
     * \bref the corresponding original computation
     */
    Computation original_cp;

    /**
     * \bref scheduled computation
     */
    Computation scheduled_cp;

    /**
     * \bref all iters, including old iters, splited iters.
     */
    std::vector<Iter> all_iters;
    /**
     * \bref current root_iters;
     */
    std::vector<Iter> root_iters;

    /**
     * \bref store the split info.
     * the range of generated outer and inner have not been setted.
     * We need pass over the split results and add external info except outer and inner iter.
     */
    std::vector<SplitResult> split_results;

    /**
     * \bref store the fuse info.
     * the generated iter starts from 0
     */
    std::vector<FuseResult> fuse_results;

    static Stage make(Computation cp);


private:
    mutable RefCount ref_count;

    template <typename T>
    friend RefCount& ref_count(const T*);
    template <typename T>
    friend void destroy(const T*);
};

template <>
inline RefCount& ref_count<StageNode>(const StageNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy<StageNode>(const StageNode* p)
{
    if(p)
        delete p;
}


class Stage : public RefCountPtr<StageNode>
{
public:
    Stage() : RefCountPtr<StageNode>() {}
    explicit Stage(StageNode* p) : RefCountPtr<StageNode>(p) {}

    /**
     * \bref split x to outer*factor + inner
     * outer and inner will be setted in the function
     * a SplitResult will be added to split_results
     */
    Stage& split(Iter x, Iter& outer_ref, Iter& inner_ref, Expr factor);
    Stage& fuse(Iter x1, Iter x2, Iter& target);
    Stage& reorder(const std::vector<Iter>& ordered_iters);
    Stage& parallel(const Iter iter);
    Stage& vectorization(const Iter iter);

};

class ScheduleNode final
{
public:
    /**
     * \bref stages in the schedule
     */
    std::vector<Stage> stages;

    /***
     * \bref record the relation of computation to stage
     */
    std::unordered_map<Computation, Stage> cp2stage;

    static Schedule make(std::vector<Computation> cps);
    
    static Schedule make(std::vector<Stage> ss);

    static Schedule make();

private:
    mutable RefCount ref_count;

    template <typename T>
    friend RefCount& ref_count(const T*);
    template <typename T>
    friend void destroy(const T*);

};

template <>
inline RefCount& ref_count<ScheduleNode>(const ScheduleNode* p)
{
    return p->ref_count;
}

template <>
inline void destroy<ScheduleNode>(const ScheduleNode* p)
{
    if(p)
        delete p;
}

class Schedule : public RefCountPtr<ScheduleNode>
{
public:
    Schedule() : RefCountPtr<ScheduleNode>() {}
    explicit Schedule(ScheduleNode* p) : 
        RefCountPtr<ScheduleNode>(p)
    {}

    static Schedule empty_schedule() { return ScheduleNode::make(); }

    Stage addComputation(Computation cp);

    Stage& operator[](Computation cp) const
    {
        CHECK_IF(get()->cp2stage.count(cp), "no such cp");
        return (get()->cp2stage)[cp];
    }
};


} // namespace SC


#endif
