#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

void *thread_body(void *arg)
{
    int local_var = 0;

    printf("Thread1: %p\n",(void *)&local_var);

    return NULL;
}

void *thread_body2(void *arg)
{
    int local_var = 0;

    printf("Thread2: %p\n",(void *)&local_var);

    return NULL;
}

int main(int argc, char **argv) {
        
    pthread_t thread1,thread2;

    pthread_create(&thread1, NULL, thread_body, NULL);

    size_t buffer_len = PTHREAD_STACK_MIN + 100;

    char *buffer = (char *)malloc(buffer_len * sizeof(char));

    pthread_attr_t attr;
    
    pthread_attr_init(&attr);

    pthread_attr_setstack(&attr, buffer, buffer_len);

    pthread_create(&thread2,&attr, thread_body2, NULL);

    printf("Main thread Stack: %p\n",(void *)&buffer_len);
    printf("Main thread heap : %p\n",(void *)buffer);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    free(buffer);


    return 0;
}
