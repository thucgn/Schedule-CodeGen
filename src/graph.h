/*************************************************************************
	> File Name: graph.h
	> Author: 
	> Mail: 
	> Created Time: Fri 26 Jul 2019 03:27:22 AM UTC
 ************************************************************************/

#ifndef _GRAPH_H
#define _GRAPH_H

#include <unordered_set>
#include <algorithm>
#include "hash.h"
#include "computation.h"

namespace SC
{

/**
 * \bref if cp is not visited, dfs visit cp
 * \param results store the visited computation in topological order
 */
void DFS_visit(const Computation& cp, 
        std::unordered_set<Computation>& visited, 
        std::vector<Computation>& results);

/**
 * \bref recurrently get all computations refered by the cps, and return the topological order
 */
std::vector<Computation> topological_sort(const std::vector<Computation>& cps);

};

#endif
