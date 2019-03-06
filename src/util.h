/*************************************************************************
	> File Name: util.h
	> Author: cgn
	> Mail: 
	> Created Time: 
 ************************************************************************/

#ifndef _AT_UTIL_H
#define _AT_UTIL_H

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <cmath>
#include <memory>



namespace SC{

#define LOG(format, ...) do { \
    printf("[%s, %s:%d %s] " format "\n", \
        __TIME__, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
}while(0)

#define LOGINFO(message) do { \
    printf("[%s, %s:%d %s] %s\n", \
        __TIME__, __FILE__, __LINE__, __FUNCTION__, message); \
}while(0)

#define CHECK(flag) do { \
    if(!(flag)) { \
      printf("[%s, %s:%d %s] check failure: %s \n", \
        __TIME__, __FILE__, __LINE__, __FUNCTION__, #flag); \
        exit(0); \
    } \
} while(0)

#define CHECK_IF(flag, message) do { \
    if(!(flag)) { \
      printf("[%s, %s:%d %s] check failure: %s %s\n", \
        __TIME__, __FILE__, __LINE__, __FUNCTION__, #flag, message); \
        exit(0); \
    } \
} while(0)

#define MARK_TIME(t) gettimeofday(&t, NULL)
#define DIFF_TIME(tt, ts) (((tt).tv_sec-(ts).tv_sec) + ((tt).tv_usec*1e-6 - (ts).tv_usec*1e-6))

typedef struct timeval TIME_T;

#if __cplusplus < 201103L
#error "c++ version must be higher than c++11"
#endif //201103L

#if __cplusplus < 201402L

/** 
 * \bref implementation of Stephan T.Lavavej(also known by STL)
 * \defgroup make_unique make_unique
 * \{
 */
template<class T> struct _Unique_if {
    typedef std::unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
    typedef std::unique_ptr<T[]> _Unknown_bound;
};

template<class T, size_t N> struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};

template<class T, class... Args>
    typename _Unique_if<T>::_Single_object
    make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

template<class T>
    typename _Unique_if<T>::_Unknown_bound
    make_unique(size_t n) {
        typedef typename std::remove_extent<T>::type U;
        return std::unique_ptr<T>(new U[n]());
    }

template<class T, class... Args>
    typename _Unique_if<T>::_Known_bound
    make_unique(Args&&...) = delete;
/**
 * \}
 */
#endif // 201402L
    
} // SC

#endif // _AT_UTIL_H

