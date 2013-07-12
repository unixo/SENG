/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: common.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <map> 
#include <set>
#include <vector>

// User-defined data types
typedef std::set<std::string> StringSet;
typedef std::vector<size_t> IntVector;

/**
   @brief Merge container values, delimited by a separator
 
   The function valueMerge need an iterator associated to the container,
   an iterator that points to the end of it and a string used as
   separator; a common use of this function is to obtain a string starting 
   from a container, such as a vector or a set.
 */
template <class InputIterator, class T>
T valueMerge(InputIterator first, InputIterator last, T delim)
{    
    T value;

    while (first != last) {
        value += *first++;
        if (first != last) value += delim;
    }
    
    return value;
}

/**
   Template function to dealloc a container which owns a list 
   of pointer to object.
 */
template <typename T>
struct deletePtr : public std::unary_function<bool, T>
{
    bool operator()(T *pT) const { 
        delete pT; 
        return true; 
    }
};

/**
   A singleton is perhaps the simplest design pattern, common to many 
   programming languages and it’s the way to allow one and only one 
   instance of a class.
   
   The key to creating a singleton is to prevent the client programmer 
   from having any control over the lifetime of the object. To do this, 
   all constructors are declared private, so that we also prevent the 
   compiler from implicitly generating any constructors. Also the copy 
   constructor and assignment operator (which intentionally have no 
   implementations, as they will never be called) are declared private 
   to prevent any sort of copies being made.
 */
template <class T> class Singleton 
{
private:
    Singleton(const Singleton &);
    Singleton & operator=(const Singleton &);
    T & operator=(const T &);
    
protected:
    Singleton() {}
    virtual ~Singleton() {}
    
public:
    static T & instance() {
        static T theInstance;
        return theInstance;
    }
};

extern int debugLevel;

#define CLEAR_SCREEN_CMD      "clear"
#define LOG(L, ...)            if (L <= debugLevel) {                \
                                printf("[%s:%d] [%s] ", __FILE__, \
                                       __LINE__, __func__);       \
                                printf(__VA_ARGS__);              \
                              }
#define LOG_CTOR()            LOG(3, "ctor called\n")
#define LOG_DTOR()            LOG(3, "dtor called\n")

#endif /* __COMMON_H__ */
