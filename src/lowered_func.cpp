/*************************************************************************
	> File Name: lowered_func.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 19 Mar 2019 01:47:53 PM UTC
 ************************************************************************/

#include "lowered_func.h"
#include <atomic>
#include <string>
#include <unordered_map>
#include "ir.h"
#include "hash.h"
#include "iroperator.h"
#include <unordered_set>

namespace 
{

::SC::ForType iter2ForType(::SC::IterType it, ::SC::IterSche st)
{
    using ::SC::ForType;
    using ::SC::IterType;
    using ::SC::IterSche;
    ForType ret;
    /*switch(type)
    {
        case IterType::PRARLLEL:
        case IterType::THREAD_ID:
            ret = ForType::PARALLEL;
            break;
        case IterType::REDUCTION:
        case IterType::ORDERED:
        case IterType::OPAQUE:
            ret = ForType::SERIAL;
            break;
    }*/
    switch(st)
    {
        case IterSche::NO_SCHEDULE:
        case IterSche::VECTORIZED:
            ret = ForType::SERIAL;
            break;
        case IterSche::PARALLELED:
            ret = ForType::PARALLEL;
            break;
    }
    return ret;
}

} // namespace

namespace SC
{

class GlobalNameManager
{
private:
    std::atomic<int> var_cnt{0};
    std::atomic<int> iter_cnt{0};
    std::unordered_map<VarExpr, std::string, std::hash<VarExpr>, SC::ExprCompare> var2name;
    std::unordered_map<Iter, std::string, std::hash<Iter>, SC::IterCompare> iter2name;
public:
    bool contains(VarExpr v) const { return var2name.count(v) > 0; }
    bool contains(Iter v) const { return iter2name.count(v) > 0; }

    std::string getOrGenName(VarExpr v)
    {
        if(contains(v))
            return var2name[v];

        ++ var_cnt;
        std::string name = "var_";
        if(!v->label.empty())
            name += v->label;
        else
            name += std::to_string(var_cnt);
        var2name.insert(std::make_pair(v, name));

        return name;
    }
    std::string getOrGenName(Iter v)
    {
        if(contains(v))
            return iter2name[v];

        ++ iter_cnt;
        std::string name = std::string("iter_") + std::to_string(iter_cnt);
        iter2name.insert(std::make_pair(v, name));
        return name;
    }

};


void collect_iter_to_splitresult(
        std::unordered_map<VarExpr, int, std::hash<VarExpr>, SC::ExprCompare>& itervar2sr,
        const std::vector<SplitResult>& srs)
{
    for(unsigned i = 0;i < srs.size(); i ++)
    {
        itervar2sr.insert(std::make_pair(srs[i].outer->var, i));
        itervar2sr.insert(std::make_pair(srs[i].inner->var, i));
    }
}

/**
 * \bref just build a simple nest loop
 */
Stmt lowerStage(Stage& s)
{

    //construct for loop
    const auto& root_iters = s->root_iters;
    std::vector<Stmt> loops;
    Stmt no_op = Evaluate::make(0);
    for(auto rit=root_iters.begin(); rit != root_iters.end(); rit ++)
    {
        auto loop = For::make(
                iter2ForType((*rit)->iter_type, (*rit)->iter_sche),
                (*rit)->var,
                (*rit)->range.min,
                (*rit)->range.extent,
                no_op);
        loops.emplace_back(std::move(loop));
    }
    
    /*const auto& reduce_iters = s->original_cp->rootIters();
    for(auto rit=reduce_iters.begin(); rit != reduce_iters.end(); rit ++)
    {
        auto loop = For::make(
                iter2ForType((*rit)->iter_type),
                (*rit)->var,
                (*rit)->range.min,
                (*rit)->range.extent,
                no_op);
        loops.emplace_back(std::move(loop));
    }*/

    // deal with body of computation
    Stmt innermost_body = s->original_cp->buildBody();
    Stmt ret = innermost_body;
    if(loops.size() > 0)
    {
        //map of <itervar, splitresult&>
        std::unordered_map<VarExpr, int, std::hash<VarExpr>, SC::ExprCompare> itervar2sr;
        collect_iter_to_splitresult(itervar2sr, s->split_results);
        std::unordered_set<int> has_accessed;

        for(auto rit = loops.rbegin(); rit!=loops.rend(); rit ++)
        {
            const For* node = rit->cast_to<For>();
            // loop generated from split
            if(itervar2sr.count(node->var))
            {
                // index of sr in splitresult vector
                int index = itervar2sr[node->var];
                if(!has_accessed.count(index))
                {
                    // rebase statement
                    SplitResult& sr = s->split_results[index];
                    Stmt rebase = Store::make(sr.x, sr.outer*sr.factor+sr.inner);
                    Stmt bound = IfThenElse::make(
                            sr.x < sr.outer.upperBound(),
                            ret, Stmt());
                    ret = Block::make(rebase, bound);
                    // record
                    has_accessed.insert(index);
                }
            }
            ret = For::make(
                    node->for_type,
                    node->var,
                    node->min,
                    node->extent,
                    ret);
        }    
    }

    return ret;
}

Stmt buildBody(Schedule& s)
{
    if(s->stages.size() == 1)
        return lowerStage(s->stages[0]);

    Stmt body = Block::make(
            lowerStage(s->stages[0]),
            lowerStage(s->stages[1]));
    for(unsigned i = 2; i < s->stages.size(); i ++)
    {
        body = Block::make(
                body,
                lowerStage(s->stages[i]));
    }
    return body;
}

LoweredFunc lower(Schedule s)
{
    Stmt body = buildBody(s); 
    
    LoweredFuncNode* n = new LoweredFuncNode();
    n->body = std::move(body);
    return LoweredFunc(n);
}

} // namespace SC

