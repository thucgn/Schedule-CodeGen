/*************************************************************************
	> File Name: tensor.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 18 Mar 2019 07:16:35 AM UTC
 ************************************************************************/

#include "tensor.h"
#include "util.h"
#include "ir.h"

namespace SC
{

Tensor TensorNode::make(const std::string& name,
        DataType type, std::vector<Expr> shape, 
        Computation source_computation, int source_output_index, TensorLoc loc)
{
    CHECK_IF(shape.size() > 0, "the shape of tensor cannot be empty");
    TensorNode* n = new TensorNode();
    n->name = name;
    n->data_type = type;
    n->shape = std::move(shape);
    n->source_cps.emplace_back(std::move(source_computation));
    //n->source_cp = std::move(source_computation);
    n->source_output_index = source_output_index;
    n->location = loc;
    return Tensor(n);
}

Tensor::Tensor(const std::string& name, 
        std::vector<Expr> shape,
        DataType type,
        TensorLoc loc)
    :Tensor(TensorNode::make(name,
                type, shape, Computation(), 0, loc))
{}

Tensor::Tensor(const std::string& name,
        std::vector<Expr> shape,
        Computation source_cp,
        int source_output_index,
        DataType type,
        TensorLoc loc):
    Tensor(TensorNode::make(name, type,shape,source_cp,source_output_index,loc))
{}

Expr Tensor::operator()(std::vector<Expr> indices) const
{
    CHECK_IF(indices.size()==ndim(), "indices size mismatch the ndim of tensor");
    Expr n = Call::make(
            get()->data_type,
            CallType::TENSOR_ACCESS,
            get()->name,
            std::move(indices),
            *this);
    return n; 
}


} // namespace SC

