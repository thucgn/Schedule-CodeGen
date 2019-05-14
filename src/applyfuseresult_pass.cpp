/*************************************************************************
	> File Name: applyfuseresult_pass.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 14 May 2019 02:52:24 AM UTC
 ************************************************************************/

#include "applyfuseresult_pass.h"
#include "schedule.h"
#include "iroperator.h"
#include "irtools.h"

namespace SC
{

ApplyFuseResultPass::ApplyFuseResultPass(const std::vector<FuseResult>& fuseresults)
    : frs(fuseresults)
{
    for(unsigned i = 0;i < fuseresults.size(); i ++)
    {
        itervar2fr.insert(std::make_pair(fuseresults[i].target->var, i));
    }
}

Stmt ApplyFuseResultPass::mutateNode(const For* n)
{
    Stmt new_body = mutate(n->body);
    
    if(itervar2fr.count(n->var))
    {
        int index = itervar2fr[n->var];
        auto& fr = frs[index];
        // add rebase node
        Stmt rebase_x1 = Store::make(fr.x1->var, fr.target->var/fr.x2->var);
        Stmt rebase_x2 = Store::make(fr.x2->var, fr.target->var%fr.x1->var);

        return For::make(n->for_type, n->var, n->min, n->extent, SC::collect_blocks(rebase_x1, rebase_x2, new_body));
    }
    else if(new_body.sameAs(n->body))
        return n;
    else 
        return For::make(n->for_type, n->var, n->min, n->extent, new_body);
}

} // namespace SC
