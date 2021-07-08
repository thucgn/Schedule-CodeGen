/*************************************************************************
	> File Name: refcountptr.h
	> Author: cgn
	> Mail: 
	> Created Time: 
 ************************************************************************/

#ifndef _REFCOUNTPTR_H
#define _REFCOUNTPTR_H

#include <cstdlib>
#include <atomic>
#include "util.h"

namespace SC
{

class RefCount
{
private:
    std::atomic<int> count; 
public:
    RefCount() : count(0) {}
    int incre() { return ++count; }
    int decre() { return --count; }
    bool is_zero() { return count == 0; }
};



template <typename T>
RefCount &ref_count(const T* t);

template <typename T>
void destroy(const T* t);

template <typename T>
class RefCountPtr 
{
protected : 
    T* ptr;

private : 
    void incref(T* p)
    {
        if(p)
            ref_count(p).incre();
    }
    void decref(T* p)
    {
        if(p)
            if(ref_count(p).decre() == 0)
                destroy(p);
    }
public:

    ~RefCountPtr() 
    {
        decref(ptr);
    }

    RefCountPtr() : ptr(nullptr) {}
    RefCountPtr(T* p) : ptr(p) { incref(ptr); }
    RefCountPtr(const RefCountPtr& o)
    {
        ptr = o.ptr;
        incref(ptr);
    }
    RefCountPtr(RefCountPtr&& o)
    {
        ptr = o.ptr;
        o.ptr = nullptr;
    }
    RefCountPtr& operator=(const RefCountPtr& o)
    {
        if(o.ptr == ptr)
            return *this;
        T* temp = o.ptr;
        incref(temp);
        decref(ptr);
        ptr = temp;
        return *this;
    }
    RefCountPtr& operator=(RefCountPtr&& o)
    {
        std::swap(ptr, o.ptr);
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    bool operator==(const RefCountPtr& o) const { return ptr == o.ptr; }
    bool sameAs(const RefCountPtr& o) const { return ptr == o.ptr; }
    bool notNull() const { return ptr != nullptr; }

};

} // namespace SC


#endif
