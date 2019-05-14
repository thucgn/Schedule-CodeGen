/*************************************************************************
	> File Name: applysplitresult_pass.h
	> Author: 
	> Mail: 
	> Created Time: Mon 13 May 2019 03:01:40 AM UTC
 ************************************************************************/

#ifndef _APPLYSPLITRESULT_PASS_H
#define _APPLYSPLITRESULT_PASS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "node.h"
#include "ir.h"
#include "irmutator.h"
#include "hash.h"

namespace SC
{

struct SplitResult;

/**
 * apply split result to nested loops
 * just add if-else condition and rebase
 */
class ApplySplitResultPass : public IRMutator
{
private:
    // itervar -> index of split result in split results
    std::unordered_map<VarExpr, int, std::hash<VarExpr>, SC::ExprCompare> itervar2sr;
    // accessed indices of split result in split results
    std::unordered_set<int> has_accessed_sr;
    //split results of stagenode
    const std::vector<SplitResult>& srs;
            
protected:
    Stmt mutateNode(const For* n) override;
public:
    ApplySplitResultPass(const std::vector<SplitResult>& splitresults);
};

} // namespace SC

#endif

