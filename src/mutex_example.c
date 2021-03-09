#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

char* g_str;

bool g_str_state_initialized = false;
pthread_mutex_t g_str_state_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_str_state_changed_cond = PTHREAD_COND_INITIALIZER;

void* thread_func1(void* thread_id_ptr) {
	g_str = "this is set by func1";
	pthread_mutex_lock(&g_str_state_lock);
	g_str_state_initialized = true;
	pthread_cond_signal(&g_str_state_changed_cond);
	pthread_mutex_unlock(&g_str_state_lock);
	return 0;
}

void* thread_func2(void* thread_id_ptr) {
	pthread_mutex_lock(&g_str_state_lock);
	while(g_str_state_initialized == false) {
		pthread_cond_wait(&g_str_state_changed_cond, &g_str_state_lock);	// put thread to sleep until it's signaled
	}
	pthread_mutex_unlock(&g_str_state_lock);
	for(int i = 0; i < 20; i++) {
		printf("%c ", g_str[i]);
	}
	puts("\n");
	return 0;
}

int mutex_example() {
	pthread_t thread_id0;
	pthread_t thread_id1;
	pthread_create(&thread_id1, NULL, thread_func2, (void*)&thread_id1);
	sleep(1); // make sure thread_func2 has plenty of time to claim the state lock
	pthread_create(&thread_id0, NULL, thread_func1, (void*)&thread_id0);

	pthread_exit(NULL);
	return 0;
}