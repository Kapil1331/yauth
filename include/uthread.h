typedef struct uthread {
	int tid;
	void *stack;
	int (*func)(void *);
	void *arg;
	struct uthread* next;
} uthread;

int uthread_create(uthread *thread, int(*func)(void *), void *arg);
int uthread_join(uthread thread);
void uthread_exit();
int uthread_cancel(uthread thread);
int uthread_kill(uthread thread);
