/*************************************************************************
	> File Name: iroperator.h
	> Author: 
	> Mail: 
	> Created Time: Thu 14 Mar 2019 09:29:44 AM UTC
 ************************************************************************/

#ifndef _IROPERATOR_H
#define _IROPERATOR_H

#include "node.h"
#include "ir.h"
#include "util.h"

namespace SC
{

inline Expr operator+(Expr a, Expr b)
{
    return Add::make(std::move(a), std::move(b));
}
inline Expr operator-(Expr a, Expr b)
{
    return Sub::make(std::move(a), std::move(b));
}
inline Expr operator*(Expr a, Expr b)
{
    return Mul::make(std::move(a), std::move(b));
}
inline Expr operator/(Expr a, Expr b)
{
    return Div::make(std::move(a), std::move(b));
}
inline Expr operator%(Expr a, Expr b)
{
    return Mod::make(std::move(a), std::move(b));
}
inline Expr min(Expr a, Expr b)
{
    return Min::make(std::move(a), std::move(b));
}
inline Expr max(Expr a, Expr b)
{
    return Max::make(std::move(a), std::move(b));
}
inline Expr operator==(Expr a, Expr b)
{
    return Eq::make(std::move(a), std::move(b));
}
inline Expr operator!=(Expr a, Expr b)
{
    return Ne::make(std::move(a), std::move(b));
}
inline Expr operator<(Expr a, Expr b)
{
    return Lt::make(std::move(a), std::move(b));
}
inline Expr operator>(Expr a, Expr b)
{
    return Gt::make(std::move(a), std::move(b));
}
inline Expr operator<=(Expr a, Expr b)
{
    return Le::make(std::move(a), std::move(b));
}
inline Expr operator>=(Expr a, Expr b)
{
    return Gt::make(std::move(a), std::move(b));
}
inline Expr operator&&(Expr a, Expr b)
{
    return And::make(std::move(a), std::move(b));
}
inline Expr operator||(Expr a, Expr b)
{
    return Or::make(std::move(a), std::move(b));
}


} // namespace SC

#endif

