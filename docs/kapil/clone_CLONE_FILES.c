#define _GNU_SOURCE		// for clone
#include <stdio.h>
#include <fcntl.h>		// for open
#include <unistd.h>     // getpid
#include <stdlib.h>		// for malloc
#include <sched.h>		// for clone
#include <signal.h>     // for SIGCHLD
#include <err.h>		// for errno
#include <sys/wait.h>	// for waitpid


//       int clone(typeof(int (void *_Nullable)) *fn,
//                 void *stack,
//                 int flags,
//                 void *_Nullable arg, ...
                 /* pid_t *_Nullable parent_tid,
                    void *_Nullable tls,
                    pid_t *_Nullable child_tid */ 

struct clone_args{
	int a;
	int fd;
};

int A(int a){
	printf("A:In func A\n");
	printf("A:%d\n", a);
	return 0;
}

int clone_Func(void *arg){
	// prints 1 when CLONE_NEWPID is set
	printf("C:Child pid : %d\n", getpid());
	
	struct clone_args *args = (struct clone_args *)arg;
	char buff[10];
	
	int end = read(args->fd, buff, 8);
	buff[end] = '\0';	
	printf("C:Data read : %s\n", buff);
	return 6;
}

int main(int argc, char *argv[]){
	
	printf("P:Parent's pid : %d\n", getpid());
	
	int fd = open("cap_file", O_RDONLY);
	char *stack, *stackTop;
	stack = malloc(1024);
	stackTop = stack + 1024;

	struct clone_args args;
	args.a = 10;
	args.fd = fd;
	printf("P:Fd in Parent: %d\n", fd);
	
	int pid = clone(clone_Func, stackTop, CLONE_NEWPID | CLONE_FILES | SIGCHLD, (void *)&args);
	if(pid == -1){
		perror("clone failed");
	}
	else{
		printf("P:Clone Successful pid : %d\n", pid);
		if(waitpid(pid, NULL, 0) == -1){
			perror("waitpid failed");
		}
	}
	// read from the offset changed by the child for the shared fd
	char buff[10];
	int end = read(fd, buff, 8);
	buff[end] = '\0';	
	printf("P:Data read from the changed offset : %s\n", buff);

}
