#ifdef IS_STATIC_INLINE
#if IS_STATIC_INLINE == 'y'
#include "function.h"
static inline void func()
{
   function();
}
#else
extern void func();
#endif
#else
extern void func();
#endif
