/*************************************************************************
	> File Name: tensor.h
	> Author: 
	> Mail: 
	> Created Time: Mon 18 Mar 2019 06:01:53 AM UTC
 ************************************************************************/

#ifndef _TENSOR_H
#define _TENSOR_H

#include <vector>
#include "type.h"
#include "iter.h"
#include "ir.h"
#include "tensor_base.h"

namespace SC
{

/*enum class TensorLoc : uint8_t { 
    MEM,
    LDM,
};*/

class ComputationNode;

/**
 * \bref reference of computation node
 * define Computation here to avoid two-way reference
 */
class Computation : public Function
{
public:
    using ContainerType = ComputationNode;
    Computation() : Function() {}
    Computation(const FunctionNode* p) : Function(p) {}

    const ComputationNode* get() const { return (const ComputationNode*)ptr; }
    const ComputationNode* operator->() const { return get(); }
    const ComputationNode& operator*() const { return *get(); }

};

using TensorLoc = HostLoc;

class Tensor;


class TensorNode final : public TensorBaseNode
{
public:
    DataType  data_type;
    /**
     * \bref the computation whose output is the tensor
     * some tensor can be attached to multiple computations, e.g., 
     * a tensor is modified by a computation
     */
    std::vector<Computation> source_cps;

    //Computation source_cp;
    /**
     * \bref the index of output of source_cp
     */
    int source_output_index{0};

    TensorLoc location{TensorLoc::MEM};

    static Tensor make(
            const std::string& name,
            DataType type, 
            std::vector<Expr> shape,
            Computation source_computation,
            int source_output_index,
            TensorLoc loc);

};


class Tensor : public TensorBase
{
public:
    using ContainerType = TensorNode;
    Tensor() : TensorBase() {}
    explicit Tensor(const TensorNode* p) : TensorBase(p) {}
    /**
     * \bref currently, we just use the name and shape
     * No bound inference
     */
    Tensor(const std::string& name,
            std::vector<Expr> shape,
            DataType type = Float(32), 
            TensorLoc loc=TensorLoc::MEM);

    Tensor(const std::string& name,
            std::vector<Expr> shape,
            Computation source_cp,
            int source_output_index=0,
            DataType type=Float(32),
            TensorLoc loc=TensorLoc::MEM);

    const TensorNode* get() const { return (const TensorNode*)ptr; }
    const TensorNode* operator->() const { return get(); }
    const TensorNode& operator*() const { return *get(); }

    void add_source_computation(Computation cp) {
        TensorNode* n = (TensorNode*)ptr;
        n->source_cps.emplace_back(std::move(cp));
    }

    const std::vector<Expr>& shape() const override { return get()->shape; }
    size_t ndim() const { return get()->shape.size(); }

    template <typename... Args>
    inline Expr operator()(Args&&... args) const
    {
        std::vector<Expr> indices{std::forward<Args>(args)...};
        return this->operator()(indices);
    }
    /**
     * \bref access the element by indices
     */
    Expr operator()(std::vector<Expr> indices) const;


    class Slice
    {
    public:
        Slice(const Tensor& tensor, std::vector<Expr> indices) :
            tensor(tensor),
            indices(indices)
        {}
        Slice(const Tensor& tensor, Expr index) :
            tensor(tensor)
        {
            indices.emplace_back(std::move(index));
        }
        
        Slice operator[](Expr index) const
        {
            std::vector<Expr> new_indices = indices;
            new_indices.emplace_back(std::move(index));
            return Slice(tensor, new_indices);
        }
        operator Expr() const
        {
            return tensor(indices);
        }

    private:
        //note:
        const Tensor& tensor;
        std::vector<Expr> indices;
    };

    Slice operator[](Expr index) const 
    {
        return Slice(*this, std::move(index));
    }

};

} // namespace SC

namespace std
{
template <>
struct hash<::SC::Tensor>
{
    std::size_t operator()(::SC::Tensor const& o) const noexcept
    {
        return std::hash<const ::SC::TensorNode*>{}(o.get());
    }    
};


} // namespace SC

#endif
