#include <stdio.h>
#ifdef IS_STATIC_INLINE
static inline void func()
{
   printf("Is static inline func.\n");
}
#else
extern void func();
#endif
