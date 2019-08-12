/*************************************************************************
	> File Name: test_gemm.cpp
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

using namespace std;
using namespace SC;

int main()
{
    int M = 1024, N = 1024, K = 1024;
    SimpleCodegenC cg;

    Tensor A = SC::placeholder("A", {M, K}, Float(32));
    Tensor B = SC::placeholder("B", {K, N}, Float(32));
    Tensor C = SC::placeholder("C", {M, N}, Float(32));

    Var bM("bM"), bN("bN"), bK("bK");

    Iter i{"i", M/bM}, j{"j", N/bN}, k{"k", K/bK};

    Computation init = SC::nest_loop_computation( "init_c", {i,j}, 
            { C[i][j] = 0.0f });

    Computation cp = SC::nest_loop_computation("dense",{i, j, k}, 
            { call_external("new_gemm", {A->name, B->name, C->name, bM*i, bN*j, bK*k, M, N, K})} );

    Schedule s(cp);

    /*LOG("scheduled gemm without optimize");
    LoweredFunc lf = lower(s);
    lf->body.accept(&cg);*/

    return 0;
}
