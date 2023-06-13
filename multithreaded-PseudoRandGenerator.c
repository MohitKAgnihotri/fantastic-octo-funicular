#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <malloc.h>


#define SINGLE_THREAD 0


#if (defined SINGLE_THREAD) && SINGLE_THREAD == 1
long
irand(long seed)
{
    long high = seed / 127773l;    /* 127773 is 2147483647 / 16807 */
    long low  = seed % 127773l;
    long test = 16807 * low - 2836l * high;   /* 2836 is 2147483647 % 16807 */
    if (test > 0l) seed = test; else seed = test + 2147483647l;
    return seed;
}


void
main()
{

    int i, k;

    long  seed, seed_ = 1l;

    int Niter = 32 * 1024 * 1024;

    int Npasses = 100;

    for(i=0;i<Npasses;i++)
    {
        seed = seed_;
        for(k=0;k<Niter;k++) seed = irand(seed);
        printf("%3d: seed value = %ld\n",i,seed);
    }


}

#else


long seeds[100] = {0};

// struct:
typedef struct data_object
{
    int thread_id;
    int seed;
    int Niter;
}data_object_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long irand(long seed)
{
    long high = seed / 127773l;    /* 127773 is 2147483647 / 16807 */
    long low  = seed % 127773l;
    long test = 16807 * low - 2836l * high;   /* 2836 is 2147483647 % 16807 */
    if (test > 0l) seed = test; else seed = test + 2147483647l;
    return seed;
}

void* thread_function(void *data)
{
    data_object_t *thread_data_object = (data_object_t *)data;
    long seed = thread_data_object->seed;
    for(int k=0;k<thread_data_object->Niter;k++)
        seed = irand(seed);
    printf("%3d: seed value = %ld\n",thread_data_object->thread_id,seed);

    pthread_mutex_lock(&mutex);
    seeds[thread_data_object->thread_id] = seed;
    pthread_mutex_unlock(&mutex);

    printf("\nThread function executed\n");
    pthread_exit(0);
}

int main (int argc, char** argv)
{

    pthread_t threadId[100] = {0};

    pthread_attr_t  pthread_attribute;


    pthread_attr_init(&pthread_attribute);

    // Create a thread to process a specific objectives
    unsigned int max_thread_num = 100;
    while(max_thread_num)
    {
        data_object_t *thread_data_object = (data_object_t *)malloc(sizeof (data_object_t) * 1);
        thread_data_object->thread_id = max_thread_num-1;
        thread_data_object->seed = 1l;
        thread_data_object->Niter = 32 * 1024 * 1024;
        pthread_create(&threadId[max_thread_num-1], &pthread_attribute, thread_function, thread_data_object);
        max_thread_num--;
    }

    // Wait for the thread to be finished
    max_thread_num = 100;
    while(max_thread_num)
    {
        pthread_join(threadId[max_thread_num-1], NULL);
        max_thread_num--;
    }

    for (int i = 0; i < 100; i++)
    {
        printf("seeds[%d] = %ld\n", i, seeds[i]);
    }

}

#endif