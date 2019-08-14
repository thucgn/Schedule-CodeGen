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
    int M, N, K;
    Computation cp;
    Iter k;
public:
    void setParameter(Param& param) override
    {
        M = std::stoi(param["M"]);
        N = std::stoi(param["N"]);
        K = std::stoi(param["K"]);
        k = Iter("k", {0,K}, IterType::REDUCTION);
        LOG("dense parameter m %d n %d k %d", M, N, K);
    }
    Computation define(Space& spa) override 
    {
        
        Tensor A = SC::placeholder("A", {M, K}, Float(32));
        Tensor B = SC::placeholder("B", {K, N}, Float(32));
        Tensor C = SC::placeholder("C", {M, N}, Float(32));
        Iter i{"i", M}, j{"j", M};//, k{"k", {0, K}, IterType::REDUCTION};
        Computation init = SC::nest_loop_computation("init_c", {i,j}, 
            { C[i][j] = 0.0f });
        cp = SC::nest_loop_computation("dense",{i, j, k}, { reduce_add(C[i][j], A[i][k]*B[k][j]) });

        Axis io, ii, jo, ji;
        std::tie(io, ii) = spa.define_split("mc", i, {2, 4, 8, 16, 32, 64});
        std::tie(jo, ji) = spa.define_split("nc", j, {2, 4, 8, 16, 32, 64});
        spa.define_reorder("reorderij", { {io, jo, ii, ji, k} });

        return cp;
    }
    void schedule(Schedule& s, Space& spa) override
    {
        Axis io, ii, jo, ji;
        Stage& stage = s[cp];
        std::tie(io, ii) = spa.apply_split("mc", stage); 
        std::tie(jo, ji) = spa.apply_split("nc", stage);
        spa.apply_reorder("reorderij", stage);
        stage.reorder({jo->x, io->x, ii->x, ji->x, k});
    }
    Operator* clone() override {
        CHECK_IF(false, "clone has not been implemented");
    }
};

} // namespace SC

#endif
