#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx;

void *thread_body(void* arg)
{
    int *shared_var_ptr = (int *)arg;
    pthread_mutex_lock(&mtx);
    (*shared_var_ptr)++;
    printf("After thread 1:%d\n",*shared_var_ptr);
    pthread_mutex_unlock(&mtx);
    return NULL;
}
void *thread_body_2(void* arg)
{
    int *shared_var_ptr = (int *)arg;
    pthread_mutex_lock(&mtx);
    (*shared_var_ptr)+=2;
    printf("After thread 2:%d\n",*shared_var_ptr);
    pthread_mutex_unlock(&mtx);
    return NULL;
}


int main(int argc, char **argv) {
   
    int shared_var = 0;
    pthread_t thread1,thread2;
    
    pthread_mutex_init(&mtx, NULL);
    
    pthread_create(&thread1, NULL, thread_body,&shared_var);
    pthread_create(&thread2, NULL, thread_body_2,&shared_var);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    
    pthread_mutex_destroy(&mtx);



    return 0;
}
