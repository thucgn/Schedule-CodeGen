/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Mar 2019 01:48:59 AM UTC
 ************************************************************************/

#include <iostream>
#include "simple_codegen.h"
#include "iroperator.h"
#include "computation.h"
#include "schedule.h"
#include "tensor.h"
#include "type.h"
#include "node.h"
#include "lowered_func.h"
using namespace std;
using namespace SC;

int main()
{
    SimpleCodegenC cg;
    Tensor A{"A", {10, 10}};
    Tensor B{"A", {10, 10}};
    Tensor C{"A", {10, 10}};

    Schedule s = Schedule::empty_schedule();
    Iter im{"i", 10}, jn{"j", 10}, kk{"k", {0,10}, IterType::REDUCTION};
    Iter imo, imi, jno, jni, outer;
    Stmt reduce = reduce_add(C[im][jn], A[im][kk]*B[kk][jn]);
    Computation cp = nest_loop_computation(
            s, "main", {im, jn, kk}, { reduce });


    // original 
    LOG("original gemm");
    LoweredFunc lf_origin = lower(s);
    lf_origin->body.accept(&cg);

    // schedule 
    s[cp].split(im, imo, imi, 2);
    s[cp].split(jn, jno, jni, 4);
    s[cp].reorder({imo, jno, imi, jni, kk});
    s[cp].fuse(imo, jno, outer);
    s[cp].parallel(outer);
    LOG("scheduled gemm without optimize");
    LoweredFunc lf = lower(s, false);
    lf->body.accept(&cg);

    // constant fold
    LOG("scheduled gemm with optimize");
    LoweredFunc lf_opt = lower(s);
    lf_opt->body.accept(&cg);

    return 0;
}


