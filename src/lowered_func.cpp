/*************************************************************************
	> File Name: lowered_func.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 19 Mar 2019 01:47:53 PM UTC
 ************************************************************************/

#include "lowered_func.h"
#include <atomic>
#include <string>
#include 
#include "ir.h"

namespace SC
{

class GlobalNameManager
{
private:
    std::atomic<int> var_cnt;
    std::atomic<int> iter_cnt;
    std::
public:
    std::string getOrGenName(VarExpr v)
    {
        if(!v->label.empty())
            return std::string("var")+v->label;
        else
        {
            ++ var_cnt;
            return std::string("var") 
                + std::to_string(var_cnt);
        }
    }
    std::string getOrGenName(Iter v)
    {
        
    }
};

/**
 * \bref just build the simple for loop
 */
Stmt lowerStage(Stage& s)
{
      
}

Stmt buildBody(Schedule& s)
{
    for(auto& stage : s->stages)
    {
                 
    }
}

LoweredFunc lower(Schedule s)
{
    Stmt body = buildBody(s); 
    
    LoweredFuncNode* n = new LoweredFuncNode();
    n->body = std::move(body);
    return LoweredFunc(n);
}

} // namespace SC

