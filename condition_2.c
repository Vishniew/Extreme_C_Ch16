#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int share_var = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

void *worker_thread(void *arg)
{
    int *share_var = (int *)arg;
    pthread_mutex_lock(&mutex);

    printf("after 1st: %d\n",++(*share_var));

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *worker_thread2(void *arg)
{
    int *share_var = (int *)arg;
    pthread_mutex_lock(&mutex);
    (*share_var) += 2;
    printf("After 2nd: %d\n",*share_var);

    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main()
{
    pthread_t thread1,thread2;
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&thread1,NULL,worker_thread,&share_var);
    pthread_create(&thread2,NULL,worker_thread2,&share_var);


    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
