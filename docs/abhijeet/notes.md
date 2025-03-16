# clone

##  general

 - clone()
   on success, thread ID of child is returned in the caller
   on failure, -1 is returned and errno is set

 - read NOTES sections in manpage (clone) 

## flags in clone

### CLONE_FILES
  - the calling process and the child process share the same file descriptor table
  - any file descriptor created by child or calling process is valid in other process
    
### CLONE_FS
  - share the info on file-system
  - anycall to chroot, chdir, umask 
  - if not set, child process works on copy of file system info at time of clone
    and call to chroot, chdir do not affect the other process

### CLONE_NEWPID
  - create new process in new PID namespace
  - PID in new namespace starts at 1
  - needs sudo previlege

### CLONE_SIGHAND
  - both processes share the same table of signal handlers
  - if not present, the child process copies the table at the time of clone
  - CLONE_VM flag is must to use CLONE_SIGHAND

### CLONE_VM
  - calling process and the child process run in the same memory space
  - memory writes by either process is visible to both
  - if this flag is not set, child process run in a seperate copy of mem space
  - what mem space ?

### CLONE_THREAD
  - the child process is placed in same thread group as calling process
  - without this, resulting thread is places in new thread group
  - CLONE_SIGHAND must be included to use CLONE_THREAD
    and CLONE_SIGHAND requires CLONE_VM
    ( CLONE_THREAD --> CLONE_SIGHAND --> CLONE_VM )
  
## pid_t
### datatype pid_t
  - resides in <sys/types.h>
  - is a signed integer

  - used to store
    - process ID
    - process group ID
    - session ID

  - two functions
    - getpid() - returns process id of calling function
    - getppid() - returns parent process id

    - another function
      gettid() - returns thread ID of current thread
      obtained by linux kernel

  - size(pid_t) = size(int)



# pthreads

## functions 

###  pthread_create()
 - creates a new thread (by invoking start_routine)
 - new thread can be terminated in following ways
   1. pthread_exit (by specifying exit status)
   2. pthread_cancel (it is cancelled by parent)
   3. pthread_join (wait for specified thread to terminate)
   4. exit (any thread in process calls exit, process dies with all its threads?)

### pthread_join()
 - waits for thread specified to terminate
 - if already dead - returns immediately
 - (the thread must be joinable)
 - *retval* is obtained by *pthread_exit*
   *(does this mean pthread_join calls pthread_exit?)*
 - if *retval* is not NULL, *retval = exit_status
 - failure to join a joinable thread produces a zombie thread
 - all of the threads are peers, any thread can join with any thread
      
### pthread_exit()
 - terminates current thread
 - returns value through *retval
   (if the thread is joinable)

### pthread_cancel()
 - sends cancellation request to specified thread
 - cancellation request is queued (if enabled, it will be accepted, else wait)
 - steps
   1. cancellation clean-up handlers are popped (popping in reverse order)
   2. thread specific destructors are called
   3. thread_exit

### pthread_self
 - obtain if of calling thread
 - thread IDs are guaranteed to be unique within process and also reused
 - thread ID from *pthread_self()* are not the same as kernel thread ID from gettid()
 - can be a arithmetic type or structure
 - use of '==' mustnt be allowed, should use *pthread_equal()* instead

### pthread_equal
 - compare thread IDs
 - (their structure if any) 

 
## Attached & Detached threads
 - in some scenarios, a thread will be created as *detached* instead of *joinable*
   once thread is detached, it can never be joined
   in that case resources are reclaimed for more efficient use

   `
   int pthread_detach(pthread_t thread);
   `
   
 - also called fire-&-forget a thread




# TODO

  - ( ) explore pthread libraries
  - ( ) matrix mul
  - ( ) what id ofile/nofile ?
  - ( ) change file descriptor in parent and child
  - ( ) define memory space ? structure and location
  - ( ) what is STACK used for ?
  - ( ) how to settid ?
  - ( ) getcontext, makecontext, swapcontext syscalls (check 'em out)
  - ( ) setjmp ljmp ?
  - ( ) 

