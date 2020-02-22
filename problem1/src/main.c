#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_CAT 13
#define MAX_DOG 9
#define MAX_THREAD (MAX_CAT + MAX_DOG)
#define MAX_BOWL 2
#define ABSURDLY_HIGH_NUMBER 20000

#define CAT 1
#define DOG 0
#define NONE -1

int bowl_take(void);
int bowl_free(int i);
int bowl_count();
void kitchen_stat(void);

void drink(int PET, int num);

pthread_mutex_t kitchen = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cat_cond, dog_cond;
int catCount, dogCount;
int turn = NONE;
int bowls[2];

void *Cat(void *arg);
void *Dog(void *arg);

void *Cat(void *arg) {
    int num = *(int *)arg;
    // Initialize cat
    printf("<Cat %d> Going to the kitchen, hoping I don't run into any dogs.\n", num);

    while (1) {
        if (rand() % ABSURDLY_HIGH_NUMBER == 0) { // I'm a firm believer of self-documenting code
            // drink/wait
            int currbowl = bowl_take();
            if(currbowl == -1) {
                continue;
            }

            pthread_mutex_lock(&kitchen);
            catCount++;
                        
            while (dogCount > 0) {
                catCount--;
                printf("<Cat %d> Waiting for dogs to clear.\n", num);
                pthread_cond_wait(&dog_cond, &kitchen);
            }
            bowl_free(currbowl);
            while(bowl_count() == 0) {
                printf("<Cat %d> Waiting for bowls. Bowls left: %d\n", num, bowl_count());
                pthread_cond_wait(&cat_cond, &kitchen);
            }
            currbowl = bowl_take();
            kitchen_stat();
            printf("<Cat %d> Picked bowl %d\n", num, currbowl);
            drink(CAT, num);
            catCount--;
            bowl_free(currbowl);
            printf("<Cat %d> Leaving kitchen.\n\n", num);
            // pthread_cond_signal(&cat_cond);
            pthread_cond_signal(&dog_cond);
            pthread_mutex_unlock(&kitchen);
        } 
        // Sleep for a decent amount before trying again
        // usleep(500);
    }
    pthread_exit(NULL);
}

void *Dog(void *arg) {
    int num = *(int *)arg;
    // Initialize dog
    printf("<Dog %d> Going to get water, hope I don't run into any cats.\n", num);
    
    while (1) {
        if (rand() % ABSURDLY_HIGH_NUMBER == 0) {
            int currbowl = bowl_take();
            if(currbowl == -1) {
                continue;
            }

            pthread_mutex_lock(&kitchen);
            dogCount++;
            
            while (catCount > 0) {
                dogCount--;
                printf("<Dog %d> Waiting for cats to clear.\n", num);
                pthread_cond_wait(&cat_cond, &kitchen);
            }
            bowl_free(currbowl);
            while (bowl_count() == 0) {
                printf("<Dog %d> Waiting for bowls. Bowls left: %d\n", num, bowl_count());
                pthread_cond_wait(&dog_cond, &kitchen);
            }
            currbowl = bowl_take();
            kitchen_stat();
            printf("<Dog %d> Picked bowl %d\n", num, currbowl);
            drink(DOG, num);
            dogCount--;
            bowl_free(currbowl);
            printf("<Dog %d> Leaving kitchen.\n\n", num);
            pthread_cond_signal(&dog_cond);
            // pthread_cond_signal(&cat_cond);
            pthread_mutex_unlock(&kitchen);
        }
        // Sleep for a decent amount before trying again 
        // usleep(500);
    }
    pthread_exit(NULL);
}

void kitchen_stat(void) {
    printf("<KITCHEN> ");
    // Let's hope it never comes to this
    if (dogCount > 0 && catCount > 0) {
        printf("\nALERT: IMMINENT FIGHTS INCOMING!\n");
        printf("==============================================\n");
        printf("       MEOW!!!!\n");
        printf("                               ARF\n");
        printf("WOLF\n");
        printf("                                            KAPOW!\n");
        printf("@!#*!)$!(#@*$!@)($!\n");
        printf("==============================================\n\n");
        return;
    } else {
        printf("All is well in the kitchen. Drinking bowls free: %d\n", bowl_count());
    }
}

void drink(int PET, int num) {
    char *name = (PET == CAT) ? "<Cat" : "<Dog";
    printf("%s %d> Drinking water\n", name, num);
    sleep(1);
    printf("%s %d> Finished drinking\n", name, num);
}

// Count the available bowls.
int bowl_count() {
    int cnt = 0;
    for(int i = 0; i < MAX_BOWL; i++) {
        if(bowls[i] == 1) {
            cnt++;
        }
    }

    return cnt;
}

// Take the first free bowl. Returns -1 if no bowls found
int bowl_take() {
    for(int i = 0; i < MAX_BOWL; i++) {
        // If free bowl
        if(bowls[i] == 1) {
            // Return bowl index, set to 0 
            bowls[i] = 0;
            return i;
        }
    }

    return -1;
}

// Release bowl[i] 
int bowl_free(int i) {
    if (bowls[i] == 1) {
        return -1; // Error: Bowl is available.
    } else {
        bowls[i] = 1;
        return 0;
    }
}

int main() {
    srand(time(0));
    pthread_t tid[MAX_THREAD];
    pthread_cond_init(&dog_cond, NULL);
    pthread_cond_init(&cat_cond, NULL);
    catCount = dogCount = 0;
    for(int i = 0; i < MAX_BOWL; i++) {
        bowls[i] = 1; // Free bowls
    }
    kitchen_stat();

    for (int id = 0; id < MAX_THREAD; id++) {
        // Create cat threads
        if (id < MAX_CAT) {
            int catn = id;
            pthread_create(&tid[id], NULL, Cat, &catn);
        }
        // Create dog threads
        else {
            int dogn = id - MAX_CAT;
            pthread_create(&tid[id], NULL, Dog, &dogn);
        }
    }

    for (int id = 0; id < MAX_THREAD; id++) {
        pthread_join(tid[id], NULL);
    }

    return 0;
}