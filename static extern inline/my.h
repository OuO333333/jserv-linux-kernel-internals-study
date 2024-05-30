#include <stdio.h>
#ifdef IS_STATIC_INLINE
#include "function.h"
static inline void func()
{
   function();
}
#else
extern void func();
#endif
