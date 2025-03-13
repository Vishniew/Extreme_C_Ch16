#include <stdio.h>
#include <pthread.h>

pthread_barrier_t bar;

void *thread_body(void* arg)
{
    int *shared_var_ptr = (int *)arg;
    (*shared_var_ptr)++;
    printf("After thread 1:%d\n",*shared_var_ptr);
    pthread_barrier_wait(&bar);
    return NULL;
}
void *thread_body_2(void* arg)
{
    int *shared_var_ptr = (int *)arg;
    pthread_barrier_wait(&bar);
    (*shared_var_ptr)+=2;
    printf("After thread 2:%d\n",*shared_var_ptr);
    return NULL;
}


int main(int argc, char **argv) {
   
    int shared_var = 0;
    pthread_t thread1,thread2;
    
    pthread_barrier_init(&bar, NULL,2);
    
    pthread_create(&thread1, NULL, thread_body,&shared_var);
    pthread_create(&thread2, NULL, thread_body_2,&shared_var);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    
    pthread_barrier_destroy(&bar);


    return 0;
}
