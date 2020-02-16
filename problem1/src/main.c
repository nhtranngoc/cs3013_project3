#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CAT 13
#define MAX_DOG 9
#define MAX_THREAD (MAX_CAT + MAX_DOG)

pthread_mutex_t dog_kitchen;
pthread_mutex_t cat_kitchen;

void *Cat(void *arg);
void *Dog(void *arg);

void *Cat(void *arg) {
    int num = *(int *)arg;
    printf("<Cat %d> Going to the kitchen, hoping I don't run into any dogs.\n", num);
    sleep(2);
    printf("<Cat %d> Finished drinking water.\n", num);
    pthread_exit(NULL);
}

void *Dog(void *arg) {
    int num = *(int *)arg;
    printf("<Dog %d> Going to get water, hope I don't run into any cats.\n", num);
    sleep(3);
    printf("<Dog %d> Finished drinking water.\n", num);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[MAX_THREAD];

    for(int id = 0; id < MAX_THREAD; id++) {
        // Create cat threads
        if(id < MAX_CAT) {
            pthread_create(&tid[id], NULL, Cat, &id);
        }
        // Create dog threads 
        else {
            int dogn = id - MAX_CAT;
            pthread_create(&tid[id], NULL, Dog, &dogn);
        }
    }

    for(int id = 0; id < MAX_THREAD; id++) {
        pthread_join(tid[id], NULL);
    }

    return 0;
}