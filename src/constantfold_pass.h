/*************************************************************************
	> File Name: ConstantFoldPass.h
	> Author: 
	> Mail: 
	> Created Time: Fri 29 Mar 2019 08:23:58 AM UTC
 ************************************************************************/

#ifndef _CONSTANTFOLDPASS_H
#define _CONSTANTFOLDPASS_H

#include "node.h"
#include "ir.h"
#include "irmutator.h"

namespace SC
{

/**
 * note: overflow has not been handled
 */
class ConstantFoldPass : public IRMutator
{
protected:
    Expr mutateNode(const Add* n) override;
    Expr mutateNode(const Sub* n) override;
    Expr mutateNode(const Mul* n) override;
    Expr mutateNode(const Div* n) override;
    Expr mutateNode(const Mod* n) override;
    Expr mutateNode(const Min* n) override;
    Expr mutateNode(const Max* n) override;
};

/*
 * check if a can be divided by b
 */
bool check_divided(Expr a, Expr b);

} // namespace SC

#endif
