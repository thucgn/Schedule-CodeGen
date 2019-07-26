/*************************************************************************
	> File Name: test_direct.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 22 Jul 2019 04:24:55 AM UTC
 ************************************************************************/

#include <iostream>

#include "operator.h"
#include "simple_codegen.h"
#include "iroperator.h"
#include "computation.h"
#include "schedule.h"
#include "tensor.h"
#include "type.h"
#include "node.h"
#include "lowered_func.h"
#include "space.h"
#include "print_info.h"
#include "graph.h"

using namespace std;
using namespace SC;

int main()
{
    int M = 1000, N = 1000, K = 1000;
    //Schedule s = Schedule::empty_schedule();
    SimpleCodegenC cg;
    Tensor A = SC::placeholder("A", {M, K}, Float(32));
    Tensor B = SC::placeholder("B", {K, N}, Float(32));
    Tensor C = SC::placeholder("C", {M, N}, Float(32));

    Iter i{"i", M}, j{"j", N}, k{"k", {0, K}, IterType::REDUCTION};

    Computation init = SC::nest_loop_computation("init_c", {i,j}, 
            { C[i][j] = 0.0f });


    Computation cp = SC::nest_loop_computation("dense",{i, j, k}, { reduce_add(C[i][j], A[i][k]*B[k][j]) });

    std::vector<Computation> cps;
    cps.push_back(cp);
    Schedule s(cps);

    LoweredFunc lf_origin = lower(s);
    lf_origin->body.accept(&cg);

    Iter io, ii, jo, ji, outer;
    s[cp].split(i, io, ii, 2);
    s[cp].split(j, jo, ji, 4);
    s[cp].reorder({io,jo, ii, ji, k});
    s[cp].fuse(io, jo, outer);
    s[cp].parallel(outer);
    LOG("scheduled gemm without optimize");
    LoweredFunc lf = lower(s);
    lf->body.accept(&cg);

    LOG("===================================");
    for(auto& stage : s->stages)
        print_computation(stage->original_cp);

    LOG("===================================");
    std::vector<Computation> topological = topological_sort({cp, init});
    for(auto& cp : topological)
        LOG("%s ", cp->name.c_str());
    return 0;
}
