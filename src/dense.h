*************************************************************************
	> File Name: dense.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 May 2019 09:23:37 AM UTC
 ************************************************************************/

#ifndef _DENSE_H
#define _DENSE_H

#include <map>
#include <string>
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
    Iter im{"i", m}, jn{"j", n}, kk{"k", k, IterType::REDUCTION};
    Tensor A{"A", {m, k}};
    Tensor B{"B", {k, n}};
    Tensor C{"C", {m, n}};
public:
    void setParameter() override;
    void define(Schedule& sche, Space& spa) override 
    {
        //Schedule s = Schedule::empty_schedule();
        Stmt reduce = reduce_add(C[im][jn], A[im][kk]*B[kk][jn]);
        cp = nest_loop_computation(sche, "main", {im, jn, kk}, {reduce});
        spa.define_split("mc", {2, 4, 8, 16, 32, 64});
        spa.define_split("nc", {2, 4, 8, 16, 32, 64});
    }
    void schedule(Schedule& s) override
    {
        Iter imo, imi;
        s[cp].split(im, imo, imi, 2);
    }
    Operator* clone() override;
};

} // namespace SC

#endif
