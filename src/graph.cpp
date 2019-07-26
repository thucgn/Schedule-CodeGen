/*************************************************************************
	> File Name: graph.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 26 Jul 2019 07:59:14 AM UTC
 ************************************************************************/

#include "graph.h"

namespace SC
{

void DFS_visit(const Computation& cp, 
        std::unordered_set<Computation>& visited, 
        std::vector<Computation>& results)
{
    if(visited.count(cp))
        return;

    visited.insert(cp);

    //dfs computations of input tensors
    for(auto& t : cp->input_tensors())
    {
        // first find the position of cp in source_cps of t
        unsigned i;
        for(i = 0;i < t->source_cps.size(); i++)
            if(t->source_cps[i].sameAs(cp))
                break;

        // now i is the position of cp , or the size of source computations of t
        
        for(unsigned j = 0;j < i; j ++)
            DFS_visit(t->source_cps[j], visited, results);

    }

    //dfs computations of output tensors, because output tensors may be generated from previous computation
    for(auto& t : cp->output_tensors())
    {
        // first find the position of cp in source_cps of t
        unsigned i;
        for(i = 0;i < t->source_cps.size(); i++)
            if(t->source_cps[i].sameAs(cp))
                break;

        // now i is the position of cp , or the size of source computations of t
        
        for(unsigned j = 0;j < i; j ++)
            DFS_visit(t->source_cps[j], visited, results);
    }

    results.push_back(cp);
}

std::vector<Computation> topological_sort(const std::vector<Computation>& cps)
{
    std::unordered_set<Computation> visited;
    std::vector<Computation> results;


    for(auto& cp : cps)
    {
        DFS_visit(cp, visited, results);
    }

    return results;
}

} // namespace SC

