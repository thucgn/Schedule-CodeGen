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

namespace SC
{

template <typename DT> // DT means data type, e.g., float, double
class Dense : public Operator
{
private:
    DT* a, b, c;
    int m, n, k;
public:
    void setParameter() override;
    void define(Schedule& s) override 
    {
        Tensor A{"A", {m, k}};
        Tensor B{"B", {k, n}};
        Tensor C{"C", {m, n}};

        Schedule s = Schedule::empty_schedule();
        Iter im{"i", m}, jn{"j", n}, kk{"k", k, IterType::REDUCTION};
        Iter imo, imi, jno, jni, outer;
        Stmt reduce = reduce_add(C[im][jn], A[im][kk]*B[kk][jn]);
        Computation cp = nest_loop_computation(s, "main", {im, jn, kk}, {reduce});
    }
    void schedule(Schedule& s) override
    {
        s[cp].split(im, imo, imi, 2);
        s[cp].split(jn, jno, jni, 4);
        s[cp].reorder({imo, jno, imi, jni, kk});
        s[cp].fuse(imo, jno, outer);
        s[cp].parallel(outer);

        LoweredFunc lf = lower(s);
        lf->body.accept(&cg);
    }
    Operator* clone() override;
};

} // namespace SC

#endif
