/*************************************************************************
	> File Name: computation.h
	> Author: 
	> Mail: 
	> Created Time: Mon 11 Mar 2019 06:40:45 AM UTC
 ************************************************************************/

#ifndef _COMPUTATION_H
#define _COMPUTATION_H

#include "function.h"
#include "iter.h"
#include <vector>


namespace SC
{

class Computation;


/**
 * \bref computation refers to a standalone computation block, 
 * like loop-based computing code.
 */
class ComputationNode : public FunctionNode
{
public:
    ComputationNode(FunctionNodeType type) : FunctionNode(type) {}
    virtual ~ComputationNode() {}
    /**
     * \bref root iters represents the outer most loops
     */
    virtual const std::vector<Iter>& rootIters() const = 0; 
};

/**
 * \bref reference of computation node
 */
class Computation : public Function
{
public:
    Computation() : Function() {}
    Computation(const ComputationNode* p) : Function(p) {}

    const ComputationNode* get() const { return (const ComputationNode*)ptr; }
    const ComputationNode* operator->() const { return get(); }
    const ComputationNode& operator*() const { return *get(); }
};

template <typename T>
class CPNode : public ComputationNode
{
public:
    CPNode() : ComputationNode(T::_node_type) {}
    virtual ~CPNode() {}
};

/**
 * \bref nest loop computations
 */
class NestLoopComNode : public CPNode<NestLoopComNode>
{
public:
    /**
     * \bref label
     */
    //std::string label;
    /**
     * \bref root iters represent the outer most loops
     */
    std::vector<Iter> root_iters;

    /**
     * \bref reduction iters in body
     */
    std::vector<Iter> reduce_iters;

    /**
     * \bref body, body is not a stmt, but {Expr ...}.
     * it will be lowered to multiple statements
     */
    //std::vector<Expr> body;
    
    /**
     * bref body
     */
    std::vector<Stmt> body;
    //Stmt body;


    static const FunctionNodeType _node_type = FunctionNodeType::NEST_LOOP;

    const std::vector<Iter>& rootIters() const override{ return root_iters; }

    static Computation make(const std::string& name, 
            std::vector<Iter> root_iters, 
            std::vector<Iter> reduce_iters,
            std::vector<Stmt> body);
    static Computation make(const std::string& name,
            std::vector<Iter> root_iters,
            std::vector<Stmt> body);
};


} // namespace SC


#endif
