#if IS_STATIC_INLINE == 'y'
#include "function.h"
static inline void func()
{
   function();
}
#else
extern void func();
#endif