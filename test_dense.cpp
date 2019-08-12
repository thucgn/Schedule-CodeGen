/*************************************************************************
	> File Name: test_dense.cpp
	> Author: 
	> Mail: 
 ************************************************************************/

#include <iostream>
#include <map>
#include "simple_codegen.h"
#include "iroperator.h"
#include "computation.h"
#include "schedule.h"
#include "tensor.h"
#include "type.h"
#include "node.h"
#include "lowered_func.h"
#include "space.h"
#include "dense.h"
using namespace std;
using namespace SC;

int main()
{
    SimpleCodegenC cg;

    LOG("sch spa");
    //Schedule sch = Schedule::empty_schedule();
    Space spa = new SpaceNode();

    LOG("dense");

    Dense<float> dense;
    std::map<std::string, std::string> param = {{"m","257"}, {"n","257"},{"k","256"}};
    dense.setParameter(param);
    LOG("define");
    Computation cp = dense.define(spa);
    LOG("schedule");
    Schedule sch(cp);
    dense.schedule(sch, spa);

    // constant fold
    LOG("scheduled gemm with optimize");
    LoweredFunc lf_opt = lower(sch);
    lf_opt->body.accept(&cg);

    return 0;
}


