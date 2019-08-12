/*************************************************************************
	> File Name: test_conv.cpp
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

using namespace std;
using namespace SC;

int main()
{
    int R = 1024, C = 1024, B = 128, Ni = 128, No = 128, K = 3;
    Schedule s = Schedule::empty_schedule();
    SimpleCodegenC cg;
    Tensor in = SC::placeholder(s, "in", {R, C, B, Ni}, Float(32));
    Tensor w = SC::placeholder(s, "w", {K, K, Ni, No}, Float(32));
    Tensor out = SC::placeholder(s, "out", {R, C, B, No}, Float(32));

    Iter cR("cR", R), cC("cC", C), cKr("cKr", K), cKc("cKc", K);

    //Computation init = SC::nest_loop_computation(s, "init_c", {i,j}, 
            //{ C[i][j] = 0.0f });

    Computation cp = SC::nest_loop_computation(s, "implicit_conv",{cR, cC, cKr, cKc}, 
            { call_external("new_gemm", {in[cR+cKr][cC+cKc], w[cKr][cKc], out[cR][cC], B*Ni,Ni*No, B*No, B*Ni, Ni*No, B*No})} );


    LoweredFunc lf_origin = lower(s);
    lf_origin->body.accept(&cg);

    /*Iter io, ii, jo, ji, outer;
    s[cp].split(i, io, ii, 2);
    s[cp].split(j, jo, ji, 4);
    s[cp].reorder({io,jo, ii, ji, k});
    s[cp].fuse(io, jo, outer);
    s[cp].parallel(outer);*/
    LOG("scheduled gemm without optimize");
    LoweredFunc lf = lower(s);
    lf->body.accept(&cg);

    return 0;
}
