#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define CHECK_RESULT(result)\
        if(result){\
            printf("Thread creation error occured.\n");\
            exit(1);\
        }

#define TRUE 1
#define FALSE 0

char *shared_array;
size_t shared_array_len;

pthread_barrier_t alloc_barrier;
pthread_barrier_t fill_barrier;
pthread_barrier_t done_barrier;

void *alloc_thread_body(void *arg){
    shared_array_len = 53;
    shared_array = (char *)malloc(shared_array_len * sizeof(char));
    
    pthread_barrier_wait(&alloc_barrier);
    return NULL;
}
void *filler_thread_body(void *arg){
    pthread_barrier_wait(&alloc_barrier);

    int even = *(int *)arg;
    
    char ch = 'a';
    size_t start_index = 1;
    if(even)
    {
        ch = 'Z';
        start_index = 0;
    }
    
    for(size_t i=start_index; i < shared_array_len; i+=2){
        shared_array[i] = even ? ch-- : ch++;
    }
    shared_array[shared_array_len - 1] = '\0';
    pthread_barrier_wait(&fill_barrier);
    return NULL;
}

void *printer_thread_body(void *arg){
    pthread_barrier_wait(&fill_barrier);
    printf(">>%s\n",shared_array);
    pthread_barrier_wait(&done_barrier);
    return NULL;
}

void *dealloc_thread_body(void *arg){
    pthread_barrier_wait(&done_barrier);
    free(shared_array);
    pthread_barrier_destroy(&alloc_barrier);
    pthread_barrier_destroy(&fill_barrier);
    pthread_barrier_destroy(&done_barrier);
}


int main(int argc, char **argv) {
    int True = TRUE,False = FALSE; 

    shared_array = NULL;

    pthread_barrier_init(&alloc_barrier,NULL,3);
    pthread_barrier_init(&fill_barrier,NULL,3);
    pthread_barrier_init(&done_barrier,NULL,2);

    pthread_t alloc_thread;
    pthread_t even_filler_thread;
    pthread_t odd_filler_thread;
    pthread_t printer_thread;
    pthread_t dealloc_thread;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&alloc_thread,&attr,alloc_thread_body,NULL);
    pthread_create(&even_filler_thread,&attr,filler_thread_body,&True);
    pthread_create(&odd_filler_thread,&attr,filler_thread_body,&False);
    pthread_create(&printer_thread,&attr,printer_thread_body,NULL);
    pthread_create(&dealloc_thread,&attr,dealloc_thread_body,NULL);


    pthread_exit(NULL);



    return 0;
}
