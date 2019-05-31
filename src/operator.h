/*************************************************************************
	> File Name: operator.h
	> Author: 
	> Mail: 
	> Created Time: Thu 23 May 2019 07:04:06 AM UTC
 ************************************************************************/

#ifndef _OPERATOR_H
#define _OPERATOR_H

#include "schedule.h"
#include "space.h"

namespace SC
{

class Parameter
{
public:

};

class Operator
{
public:
    virtual void setParameter() = 0;
    virtual void define(Schedule& s, Space& spa) = 0;
    virtual void schedule(Schedule& s) = 0;
    virtual Operator* clone() = 0;
};

} // namespace SC

#endif
