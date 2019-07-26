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

enum class AttachType : uint8_t
{
    ROOT,
    INLINE,
    SCOPE,
};

namespace SC
{

class Stage;
class StageNode;
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
    Stage& unroll(const Iter iter);
    Stage& compute_inline();
    Stage& compute_root();
    Stage& compute_at(Stage s, Iter scope);

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

    /**
     * \bref attach type
     */
    AttachType attach_type {AttachType::ROOT};
    /**
     * \bref attach stage
     */
    Stage attach_stage;
    /**
     * \bref attach iter of attach stage
     */
    Iter attach_iter;

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

    //static Schedule make(Computation cp);

    static Schedule make(std::vector<Computation> cps);
    
    static Schedule make(std::vector<Stage> ss);

    //static Schedule make();

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
protected:
    void construct_cp2stage();

public:
    Schedule() : RefCountPtr<ScheduleNode>() {}
    explicit Schedule(ScheduleNode* p) : 
        RefCountPtr<ScheduleNode>(p)
    {}

    Schedule(Computation cp) : 
        RefCountPtr<ScheduleNode>(ScheduleNode::make({cp}))
    {
        construct_cp2stage();
    }

    Schedule(std::vector<Computation> cps) :
        RefCountPtr<ScheduleNode>(ScheduleNode::make(std::move(cps)))
    {
        construct_cp2stage();
    }

    //static Schedule empty_schedule() { return ScheduleNode::make(); }

    //Stage addComputation(Computation cp);

    Stage& operator[](Computation cp) const
    {
        CHECK_IF(get()->cp2stage.count(cp), "no such cp");
        return (get()->cp2stage)[cp];
    }
};


} // namespace SC


#endif
