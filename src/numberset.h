/*************************************************************************
	> File Name: NumberSet.h
	> Author: 
	> Mail: 
	> Created Time: Thu 16 May 2019 02:17:47 PM UTC
 ************************************************************************/

#ifndef _NUMBERSET_H
#define _NUMBERSET_H

#include <vector>
#include <set>
#include <algorithm>
#include "util.h"

namespace SC
{

class NumberSet
{
public:
    virtual int next() const = 0;
    virtual bool has_next() const = 0;
    virtual int size() const = 0;
    virtual int remaining() const = 0;
    virtual void reset() const = 0;
    virtual ~NumberSet() {}
};

/**
 * number set represented in range: [min, max]
 */
class ContinuousSet : public NumberSet
{
public:
    ContinuousSet(int min, int max) : min(min), max(max), curr(min-1) {}
    int next() const override 
    {
        CHECK(curr < max);
        return ++curr;
    }
    bool has_next() const override { 
        return curr < max;
    }
    int size() const override {
        return max-min+1;
    }
    int remaining() const override {
        return std::max(0, max-curr);
    }
    void reset() const override {
        curr = min-1; 
    }
private:
    int min, max;
    // curr must be initialized with min-1;
    mutable int curr;
};

class DiscreteSet : public NumberSet
{
public:
    DiscreteSet(const std::vector<int>& candidates)
    {
        std::set<int> tmp;
        for(auto& candidate : candidates)
            if(!tmp.count(candidate) && candidate > 0)
                tmp.insert(candidate);
        CHECK_IF(tmp.size(), "no valid candidates");
        for(auto& candidate : candidates)
            if(tmp.count(candidate))
                this->candidates.push_back(candidate);
        // initialize the curr to -1;
        curr = -1;
    }
    int next() const override {
        return candidates.at(++curr);
    }
    bool has_next() const override {
        return (unsigned)(curr+1) < candidates.size();
    }
    int size() const override {
        return candidates.size(); 
    }
    int remaining() const override {
        return (int)(candidates.size()-1) - curr;
    }
    void reset() const override {
        curr = -1;
    }
private:    
    std::vector<int> candidates;
    // curr must be initialized with min-1;
    mutable int curr;
};

} // namespace SC

#endif
