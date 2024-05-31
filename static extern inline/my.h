#ifndef CONFIG_MY_H
#define CONFIG_MY_H

#if IS_STATIC_INLINE == 'y'
#include "function.h"
static inline void func()
{
   function();
}
#else
extern void func();
#endif // IS_STATIC_INLINE == 1

#endif // CONFIG_MY_H