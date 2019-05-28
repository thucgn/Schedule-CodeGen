/*************************************************************************
	> File Name: tuner.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 May 2019 07:21:35 AM UTC
 ************************************************************************/

#ifndef _TUNER_H
#define _TUNER_H

#include "space.h"

namespace SC
{

class Tuner
{
public:
    Tuner() = default;
    virtual ~Tuner() {}
    virtual void tune() = 0;
};

class SimpleTuner : public Tuner
{
public:
    SimpleTuner() {}
    virtual ~SimpleTuner() {}
    void tune() override;
private:
};

} // namespace SC

#endif
