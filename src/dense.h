/*************************************************************************
	> File Name: dense.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 May 2019 09:23:37 AM UTC
 ************************************************************************/

#ifndef _DENSE_H
#define _DENSE_H

#include <map>
#include <string>
#include <tuple>
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

namespace SC
{

template <typename DT> // DT means data type, e.g., float, double
class Dense : public Operator
{
private:
    DT* a, b, c;
    int m, n, k;
    Computation cp;
    //Iter im{"i", m}, jn{"j", n}, kk{"k", k, IterType::REDUCTION};
    Iter im, jn, kk;
    Tensor A{"A", {m, k}};
    Tensor B{"B", {k, n}};
    Tensor C{"C", {m, n}};
public:
    void setParameter(Param& param) override
    {
        m = std::stoi(param["m"]);
        n = std::stoi(param["n"]);
        k = std::stoi(param["k"]);
        im = Iter("i", m);
        jn = Iter("j", n);
        kk = Iter("k", k, IterType::REDUCTION);
        LOG("dense parameter m %d n %d k %d", m, n, k);
    }
    void define(Schedule& sche, Space& spa) override 
    {
        //Schedule s = Schedule::empty_schedule();
        Stmt reduce = reduce_add(C[im][jn], A[im][kk]*B[kk][jn]);
        cp = nest_loop_computation(sche, "main", {im, jn, kk}, {reduce});
        Axis io, ii, jo, ji;
        std::tie(io, ii) = spa.define_split("mc", im, {2, 4, 8, 16, 32, 64});
        std::tie(jo, ji) = spa.define_split("nc", jn, {2, 4, 8, 16, 32, 64});
        spa.define_reorder("reorderij", { {io, jo, ii, ji, kk} });
    }
    void schedule(Schedule& s, Space& spa) override
    {
        Axis io, ii, jo, ji;
        Stage& stage = s[cp];
        std::tie(io, ii) = spa.apply_split("mc", stage); 
        std::tie(jo, ji) = spa.apply_split("nc", stage);
        spa.apply_reorder("reorderij", stage);
        stage.reorder({jo->x, io->x, ii->x, ji->x, kk});
    }
    Operator* clone() override {
        CHECK_IF(false, "clone has not been implemented");
    }
};

} // namespace SC

#endif
