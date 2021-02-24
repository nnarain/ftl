//
// lib/new.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 21 2021
//
#ifndef FTL_LIB_NEW_HPP
#define FTL_LIB_NEW_HPP

#include <stdlib.h>

inline void * operator new(size_t size)
{
    return malloc(size);
}

inline void * operator new[](size_t size)
{
    return malloc(size);
}

inline void * operator new(size_t size, void * ptr) noexcept
{
    (void)size;
    return ptr;
}

inline void operator delete(void * ptr)
{
    free(ptr);
}

inline void operator delete[](void * ptr)
{
    free(ptr);
}

#endif // FTL_LIB_NEW_HPP
