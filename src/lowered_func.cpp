/*************************************************************************
	> File Name: lowered_func.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 19 Mar 2019 01:47:53 PM UTC
 ************************************************************************/

#include "lowered_func.h"
#include <unordered_set>
#include <atomic>
#include <string>
#include <unordered_map>
#include "ir.h"
#include "hash.h"
#include "iroperator.h"
#include "constantfold_pass.h"
#include "applysplitresult_pass.h"
#include "applyfuseresult_pass.h"

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
        case IterSche::UNROLL:
            ret = ForType::UNROLL;
            break;
    }
    return ret;
}

/**
 * \bref check a can be divided by b
 */
/*bool check_divided(SC::Expr a, SC::Expr b)
{
    SC::ConstantFoldPass cf;   
    SC::Expr new_a = a->mutate_expr(&cf);
    SC::Expr new_b = b->mutate_expr(&cf);
    if((new_a.is_type<SC::IntImm>() || new_a.is_type<SC::UIntImm>())
            &&(new_b.is_type<SC::IntImm>() || new_b.is_type<SC::UIntImm>()))
    {
        if(new_a.is_type<SC::IntImm>() && new_b.is_type<SC::IntImm>())
        {
            return (new_a.cast_to<SC::IntImm>()->value % 
                    new_b.cast_to<SC::IntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::IntImm>() && new_b.is_type<SC::UIntImm>())
        {
            return (new_a.cast_to<SC::IntImm>()->value % 
                    new_b.cast_to<SC::UIntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::UIntImm>() && new_b.is_type<SC::IntImm>())
        {
            return (new_a.cast_to<SC::UIntImm>()->value % 
                    new_b.cast_to<SC::IntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::UIntImm>() && new_b.is_type<SC::UIntImm>())
        {
            return (new_a.cast_to<SC::UIntImm>()->value % 
                    new_b.cast_to<SC::UIntImm>()->value == 0);
        }
    }

    return false;
}*/

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
    // deal with placeholder
    if(!innermost_body.notNull())
        return Stmt();
    Stmt ret = innermost_body;
    if(loops.size() > 0)
    {
        for(auto rit = loops.rbegin(); rit!=loops.rend(); rit ++)
        {
            const For* node = rit->cast_to<For>();
            ret = For::make(
                    node->for_type,
                    node->var,
                    node->min,
                    node->extent,
                    ret);
        }    
    }

    // apply split result
    ApplySplitResultPass asp(s->split_results);
    ret = ret->mutate_stmt(&asp);
    // apply fuse result
    ApplyFuseResultPass  afp(s->fuse_results);
    ret = ret->mutate_stmt(&afp);
    /*if(loops.size() > 0)
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
                    if(check_divided(sr.x->range.extent, sr.factor))
                    {
                        ret = Block::make(rebase, ret);
                    }
                    else
                    {
                        Stmt bound = IfThenElse::make(
                                sr.x < sr.x.upperBound(),
                                ret, Stmt());
                        ret = Block::make(rebase, bound);
                    }
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
    }*/

    return ret;
}

Stmt buildBody(Schedule& s)
{

    std::vector<Stmt> stmts;
    for(auto& stage : s->stages)
    {
        Stmt stmt = lowerStage(stage);
        if(stmt.notNull())
            stmts.push_back(stmt);
    }

    if(stmts.size() == 1)
        return stmts[0];

    Stmt body = Block::make(stmts[0], stmts[1]);

    for(unsigned i = 2; i < stmts.size(); i ++)
    {
        body = Block::make(
                body,
                stmts[i]);
    }


    return body;
}

LoweredFunc lower(Schedule s, bool optimize)
{
    Stmt body = buildBody(s); 
    if(optimize)
    {
        ConstantFoldPass cf;
        body = body->mutate_stmt(&cf);
    }
    
    LoweredFuncNode* n = new LoweredFuncNode();
    n->body = std::move(body);

    return LoweredFunc(n);
}

} // namespace SC

