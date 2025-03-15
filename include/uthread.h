struct uthread {
	int tid;
	void *stack;
	void *(*func)(void *);
	void *arg;
	struct uthread* next;
}

int uthread_create();
int uthread_join();
void uthread_exit();
int uthread_cancel();
int uthread_kill();

