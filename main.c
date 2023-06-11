
#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <malloc.h>


// struct:
typedef struct data_object
{
    int thread_id;
    void *data;
}data_object_t;

void* thread_function(void *data)
{
    printf("\nThread function executed\n");
    pthread_exit(0);
}

int main (int argc, char** argv)
{

    // Get the number of CPU Cores
    unsigned int num_cpu = get_nprocs();
    pthread_t threadId[16];

    printf("Print number of CPU Cores = %d", num_cpu);
    pthread_attr_t  pthread_attribute;
    data_object_t *thread_data_object = (data_object_t *)malloc(sizeof (data_object_t) * 1);

    pthread_attr_init(&pthread_attribute);

    // Create a thread to process a specific objectives
    unsigned int max_thread_num = num_cpu;
    while(max_thread_num)
    {
        pthread_create(&threadId[max_thread_num-1], &pthread_attribute, thread_function, thread_data_object);
        max_thread_num--;
    }

    // Wait for the thread to be finished
    max_thread_num = num_cpu;
    while(max_thread_num)
    {
        pthread_join(threadId[max_thread_num-1], NULL);
        max_thread_num--;
    }

}