# Userland project: Implement a multithreading library

Implement a user level multithreading library. Since it's Linux systems, the library should come with options to do  
many-one and many-many scheudling  of user threads on kernel threads. 

# Support following functions:

- thread_create() ;		// provide option to use a desired mapping.
- thread_join() ;		// parent waits for thread
- thread_exit();		// thread exit
- thread_lock();			// a spinlock
- thread_unlock();		// spin-unlock
- thread_cancel() ;		// cancel thread
- thread_mutex_lock();	// a mutex sleep-lock
- thread_mutex_unlock();
- thread_kill();			// handle SIGs  - TERM, STOP, CONT, KILL

# Imp Notes
- Use the SIGALRM/SIGVTALRM signals to do scheduling. 
- Learn setjmp and longjmp in C library, you may also use the ucontext library.
- Your testing should cover all possible ways of creating threads and race conditions.

# Marking Scheme
- Many-one: 10 Marks, Many-Many: 10 Marks.
- Automated Testing code with coverage of all sposibilities: 10 marks
- synchronization: 10 marks
- signal handling: 10 marks


# Userland: threading libraries

- Read textbook chapter on threads. 
- Read about clone() system call.
	Try clone() in all possible ways.
	Read about signals, SIGALRM, SIGVTALRM, play with them. 

- Write pthread library programs for matrix multiplication, factorial. 
- Get some clarity about flow of code, from application->library->clone and return. 
- Start designing 1-1 implementation by defining user-land functiosn on lines of pthreads.
- Finish 1-1.
- Read about setcontext(), getcontext() or alternatively about setjmp, longjmp. 
- Think of how to switch-context in user-land combining signals and context code.
- Implement many-1. 
- Implement many-many.


# Userland threads
----------------

- Implement the same functions as provided by pthreads. 
create, join, kill 

- Different implementations: same function prototype, but different code.

- one-one/mythread.h one-one/mythread.c : mythread_create(tid_t, funcptr_t f, arg_t a) 
- many-one/mythread.h many-one/mythread.c :	mythread_create(tid_t, funcptr_t f, arg_t a) 
- in both .c files you should have functions with SAME prototype! 

- If I write a program using your library, I should be able to link it with any implementation without changing my code!

- mycode.c
```
#include "mythread.h" 
int main() {
	mythread_create(t, f, x);
}
cc -I one-one mycode.c 
cc -I many-one  mycode.c 
```

# Implemetation
# one-one
```
struct thread {
	int tid;
	stack *stack;
	funcptr f; 
	kernel-thread id;
}
global array of threads, or a LL of threads; 
mythread_create(int *tid, struct attr *attr, funcptr f, void *arg) {
	allocate stack; // malloc, sbrk(), mmap
	clone(.....,f, stack,....arg,....)
	remember the thread, also add it to LL or array;	
}
mythread_join(int *tid) {
	find the thread using tid in array/LL
	wait;
}
```

# many-one 
```
struct thread {
	int tid;
	stack *stack;
	funcptr f; 
	kernel-thread id;
}
global array of threads, or a LL of threads; 
scheduler queue (queue of threads);
mythread_create(int *tid, struct attr *attr, funcptr f, void *arg) {
	set SIGALRM handler to f()
	create one thread additional for may be the scheduler! 
	allocate stack; // malloc, sbrk(), mmap
	remember the thread, also add it to LL or array;	
	also add the thread to list of threads to be scheduled
	scheduler();
}
sigalrm-handler f() {
	....
	scheduler();
}
scheduler() {
	selects a thread for execution from the array or LL of ready-threads
	save the context of the current thread;
	load the context of chosen thread
	set ALARM;  
	"run" the new chosen thread;
	// use the makecontext, getcontext, setcontext()
	// before anything, try these functions first! 
}
```
--------------
# many-many
```
struct thread {
	int tid;
	stack *stack;
	funcptr f; 
	kernel-thread id;
	context c;
}
global array of threads, or a LL of threads; 
scheduler queue (queue of threads);
struct kernel-thread {
	tid;
	func; 
}
an array or LL of kernel threads;

default number of kernel_threads = 3; 
mythread_setkthreads(int n) {
	kernel_threads = n; 
}
mythread_create(int *tid, struct attr *attr, funcptr f, void *arg) {
	set SIGALRM handler to f()
	if current number of threads < limit kernel_threads
		clone(..f..); 
	allocate stack; // malloc, sbrk(), mmap
	remember the thread, also add it to LL or array;	
	also add the thread to list of threads to be scheduled
	scheduler();
}
sigalrm-handler f() {
	....
	scheduler();
}
scheduler() {
	selects a thread for execution from the array or LL of ready-threads
	save the context of the current thread;
	load the context of chosen thread
	set ALARM;  
	"run" the new chosen thread on a selected kernel-thread;
	// use the makecontext, getcontext, setcontext()
	// before anything, try these functions first! 
	remember which threads are scheduled where and don't do duplicate scheudling; 
}



myprogram.c
mythread_create(....f,... ) // added to the queue; 
mythread_create(....g, ..)	// clone()
mythread_create(....h, ..) // does not call a clone 
```

