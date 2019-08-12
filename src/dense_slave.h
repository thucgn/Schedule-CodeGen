/*************************************************************************
	> File Name: dense_slave.h
	> Author: 
	> Mail: 
	> Created Time: Mon 08 Jul 2019 02:40:39 AM UTC
 ************************************************************************/

#ifndef _DENSE_SLAVE_H
#define _DENSE_SLAVE_H

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

template <typename DT>
class DenseSlave : public Operator
{

private:
    DT* a, b, c;
    Var M{"M"}, N{"N"}, K{"K"};
    Computation cp;
    //Var bm{"bm"}, bn{"bn"}, bk{"bk"};
    //Iter i{"i", m}, j{"j", n}, k{"k", k};
    //Tensor A{"A", {M, K}};
    //Tensor B{"B", {K, N}};
    //Tensor C{"C", {M, N}};

public:
    void setParameter(Param& param) override
    {
        M = std::stoi(param["m"]);
        N = std::stoi(param["n"]);
        K = std::stoi(param["k"]);
    }

    void define(Schedule& sche, Space& spa) override
    {
        //Stmt matmul_4_32 = Call::make(Call::RetType(), CallType::EXTERNAL,"4_32", {A,B,C,bm, bn, bk}, j
        
    }
};

} // namespace SC

#endif
