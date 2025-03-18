---------- in ./sysdeps/x86_64/nptl/tls.h

#  define THREAD_SELF \
  (*(struct pthread *__seg_fs *) offsetof (struct pthread, header.self))
# else
#  define THREAD_SELF \
  ({ struct pthread *__self;						      \
     asm ("mov %%fs:%c1,%0" : "=r" (__self)				      \
	  : "i" (offsetof (struct pthread, header.self)));	 	      \
     __self;})
# endif


---------- in nptl/pthread_self.c
#include "pthreadP.h"
#include <tls.h>

pthread_t
__pthread_self (void)
{
  return (pthread_t) THREAD_SELF;
}
libc_hidden_def (__pthread_self)
weak_alias (__pthread_self, pthread_self)


userland thread-id   -> returned by pthread_self()
kernelland thread-ed -> returned by gettid()
