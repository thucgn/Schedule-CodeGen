/*************************************************************************
	> File Name: schedule.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Mar 2019 11:00:43 AM UTC
 ************************************************************************/

#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <vector>
#include "refcountptr.h"
#include "computation.h"

namespace SC
{

class Stage;
class Schedule;

class StageNode
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
     * \bref root iters represent the outer most loops
     */
    std::vector<Iter> root_iters;

    std::vector<Iter> reduce_iters;

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


class Stage : public RefCountPtr<const StageNode>
{

};

class ScheduleNode
{

};

class Schedule : public RefCountPtr<const ScheduleNode>
{

};

} // namespace SC


#endif
