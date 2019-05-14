/*************************************************************************
	> File Name: ir_tools.h
	> Author: 
	> Mail: 
	> Created Time: Tue 14 May 2019 07:27:11 AM UTC
 ************************************************************************/

#ifndef _IR_TOOLS_H
#define _IR_TOOLS_H

#include <type_traits>
#include <vector>
#include "ir.h"
#include "util.h"

namespace SC
{

template <typename... Args>
Stmt collect_blocks(Args... args)
{
    std::vector<Stmt> stmts{std::forward<Args>(args)...};
    CHECK_IF(stmts.size() >= 2, "less than 2 statements in collect_blocks");
    int size = stmts.size();
    Stmt ret = Block::make(stmts[size-2], stmts[size-1]);
    for(int i = size-3;i >= 0; i --)
        ret = Block::make(stmts[i], ret);
    return ret;
}

} // namespace SC

#endif
