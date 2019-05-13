/*************************************************************************
	> File Name: applysplitresult_pass.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 13 May 2019 05:02:52 AM UTC
 ************************************************************************/

#include "applysplitresult_pass.h"
#include "constantfold_pass.h"
#include "schedule.h"
#include "iroperator.h"
#include "iter.h"

namespace SC
{

ApplySplitResultPass::ApplySplitResultPass(const std::vector<SplitResult>& splitresults)
    : srs(splitresults)
{
    // collect itervar2splitresult map
    for(unsigned i = 0;i < splitresults.size(); i ++)
    {
        itervar2sr.insert(
                std::make_pair(splitresults[i].outer->var,i));
        itervar2sr.insert(
                std::make_pair(splitresults[i].inner->var,i));
    }
}

Stmt ApplySplitResultPass::mutateNode(const For* n)
{
    Stmt final_body;
    Stmt new_body = mutate(n->body);
    bool changed = false;
    if(new_body.sameAs(n->body))
    {
        final_body = n->body;
        // loop generated from split
        if(itervar2sr.count(n->var))
        {
            int index = itervar2sr[n->var];    
            // has not been accessed. 
            // note that we cannot determine the access order of outer and inner loop, because reorder could change the order
            if(!has_accessed_sr.count(index))
            {
                const SplitResult& sr = srs[index];
                Stmt rebase = Store::make(sr.x, 
                        sr.outer*sr.factor+sr.inner);
                if(check_divided(sr.x->range.extent, sr.factor))
                {
                    final_body = Block::make(rebase, final_body);         
                }
                else
                {
                    Stmt bound = IfThenElse::make(sr.x < sr.x.upperBound(), final_body, Stmt());
                    final_body = Block::make(rebase, bound);
                }
                has_accessed_sr.insert(index);
                changed = true;
            }
        }

        if(changed)
        {
            return For::make(n->for_type, n->var, n->min, n->extent, final_body);
        }
        else 
            return n;
    }
    else
    {
        final_body = new_body;
        // loop generated from split
        if(itervar2sr.count(n->var))
        {
            int index = itervar2sr[n->var];    
            // has not been accessed. 
            // note that we cannot determine the access order of outer and inner loop, because reorder could change the order
            if(!has_accessed_sr.count(index))
            {
                const SplitResult& sr = srs[index];
                Stmt rebase = Store::make(sr.x, 
                        sr.outer*sr.factor+sr.inner);
                if(check_divided(sr.x->range.extent, sr.factor))
                {
                    final_body = Block::make(rebase, final_body);         
                }
                else
                {
                    Stmt bound = IfThenElse::make(sr.x < sr.x.upperBound(), final_body, Stmt());
                    final_body = Block::make(rebase, bound);
                }
                has_accessed_sr.insert(index);
            }
        }
        return For::make(n->for_type, n->var, n->min, n->extent, final_body);
    }
}


} // namespace SC

