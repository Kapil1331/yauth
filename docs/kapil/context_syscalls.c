#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

/* Userlevel context
typedef struct ucontext
  {
    unsigned long int uc_flags;
    struct ucontext *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
  } ucontext_t;
*/

/* Context to describe whole processor state.
typedef struct
  {
    gregset_t __ctx(gregs);
    // Note that fpregs is a pointer
    fpregset_t __ctx(fpregs);
    __extension__ unsigned long long __reserved1 [8];
} mcontext_t;
*/

//[file:///home/caplin/uni/ty/sem6/os/project/glibc/sysdeps/unix/sysv/linux/x86_64/./getcontext.S]

ucontext_t ctx_main, ctx_thread;

void thread_function() {
    printf("T:Thread started\n");
    swapcontext(&ctx_thread, &ctx_main);  // switch to main
    printf("T:Thread resumed\n");
}

int main() {
	// stack for thread
    char stack[8192];  
    unsigned long rip;

    getcontext(&ctx_thread);  
	
	printf("M:SP: %p\n", ctx_thread.uc_stack.ss_sp);
    printf("M:Stack Size: %zu\n", ctx_thread.uc_stack.ss_size);
    printf("M:Next Context [pointer]: %p\n", ctx_thread.uc_link); 
    printf("M:IP of main while saving context : 0x%lx\n", ctx_thread.uc_mcontext.gregs[REG_RIP]); 

    __asm__("lea (%%rip), %0" : "=r"(rip)); 
    printf("M:Current Instruction Pointer (RIP): 0x%lx\n", rip);	

	ctx_thread.uc_stack.ss_sp = stack;
    ctx_thread.uc_stack.ss_size = sizeof(stack);
    ctx_thread.uc_link = &ctx_main;  // when the thread exits, return to 
									 // the main's context
    
	makecontext(&ctx_thread, thread_function, 0);  // Point to the function which the
												   // thread will execute
	
    printf("M:Switching to ctx_thread that is thread's context\n");

/*
int swapcontext(ucontext_t *restrict oucp,
	const ucontext_t *restrict ucp);
 The swapcontext() function saves the current context in the structure pointed to by oucp,
 and then activates the context pointed to by ucp.
*/

	swapcontext(&ctx_main, &ctx_thread);	// switch to the context of ctx_thread set up above
											// and save the current context to ctx_main

	// after calling swapcontext() in thread_func 
	printf("M:Back in main\n");
	swapcontext(&ctx_main, &ctx_thread);  // Resume thread_function
    printf("M:Main function exiting\n");

    return 0;
}
