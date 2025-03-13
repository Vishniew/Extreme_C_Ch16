#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>

sem_t semaphore;

void *thread_body(void *arg)
{
    sem_wait(&semaphore);
    printf("Thread %ld entered critical section\n",(long)arg+1);
    
    sleep(2);

    printf("Thread %ld exited critical section\n",(long)arg+1);
    
    sem_post(&semaphore);
    return NULL;
}

int main(int argc, char **argv) {
    
    pthread_t thread[3];

    sem_init(&semaphore, 0, 2);

    for(long int i=0;i<3;i++){
        pthread_create(&thread[i], NULL, thread_body, (void *)i);
    }

    for(long int i=0;i<3;i++){
        pthread_join(thread[i], NULL);
    }

    sem_destroy(&semaphore);

    return 0;
}
