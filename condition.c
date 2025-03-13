#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

//Structure to keep all the variables related to shared state

typedef int bool_t;

typedef struct{
    bool_t done;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

}shared_state_t;

void shared_state_init(shared_state_t *shared_state)
{
    shared_state -> done = FALSE;
    pthread_mutex_init(&shared_state -> mutex, NULL);
    pthread_cond_init(&shared_state -> cond,NULL);
}
void shared_state_destroy(shared_state_t *shared_state)
{
    pthread_mutex_destroy(&shared_state -> mutex);
    pthread_cond_destroy(&shared_state -> cond);
}

void *thread_body1(void *arg)
{
    shared_state_t *ss = (shared_state_t *)arg;

    pthread_mutex_lock(&ss -> mutex);

    printf("A\n");
    
    ss -> done = TRUE;  

    pthread_cond_signal(&ss -> cond);

    pthread_mutex_unlock(&ss -> mutex);
    return NULL;
}

void *thread_body2(void *arg)
{

    shared_state_t *ss = (shared_state_t *)arg;

    pthread_mutex_lock(&ss -> mutex);

    while(ss -> done == FALSE)
    {
        pthread_cond_wait(&ss -> cond, &ss -> mutex);
    }
    printf("B\n");
    pthread_mutex_unlock(&ss -> mutex);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    shared_state_t shared_state;
    
    shared_state_init(&shared_state);

    pthread_create(&thread1, NULL, thread_body1, &shared_state);
    pthread_create(&thread2, NULL, thread_body2, &shared_state);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    shared_state_destroy(&shared_state);

}

