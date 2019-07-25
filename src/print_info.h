/*************************************************************************
	> File Name: print_info.h
	> Author: 
	> Mail: 
	> Created Time: Thu 25 Jul 2019 08:36:59 AM UTC
 ************************************************************************/

#ifndef _PRINT_INFO_H
#define _PRINT_INFO_H

#include <string>
#include "schedule.h"
#include "computation.h"
#include "util.h"

namespace SC
{

void print_computation(const Computation& cp)
{
    std::string str;
    for(auto& t : cp->inputTensors)
        str.append(t->name).append(", ");
    LOG("cp name %s  input %s", cp->name.c_str(), str.c_str());

    str.clear();
    for(auto& t : cp->outputTensors)
        str.append(t->name).append(", ");
    LOG("cp name %s output %s", cp->name.c_str(), str.c_str());
}

} // namespace SC

#endif
