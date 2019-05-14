/*************************************************************************
	> File Name: applyfuseresult_pass.h
	> Author: 
	> Mail: 
	> Created Time: Tue 14 May 2019 02:39:36 AM UTC
 ************************************************************************/

#ifndef _APPLYFUSERESULT_PASS_H
#define _APPLYFUSERESULT_PASS_H

#include <unordered_map>
#include <vector>
#include "node.h"
#include "ir.h"
#include "irmutator.h"
#include "hash.h"

namespace SC
{

struct FuseResult;

/**
 * apply fuse result to nested loops
 * just add rebase
 */
class ApplyFuseResultPass : public IRMutator
{
private:
    // itervar -> index of fuse result in fuse results
    std::unordered_map<VarExpr, int, std::hash<VarExpr>, SC::ExprCompare> itervar2fr;    
    // fuse results of stagenode
    const std::vector<FuseResult>& frs;
protected:
    Stmt mutateNode(const For* n) override;

public:
    ApplyFuseResultPass(const std::vector<FuseResult>& fuseresults);

};

} // namespace SC

#endif
