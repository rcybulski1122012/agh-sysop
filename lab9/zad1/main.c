#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define NUMBER_OF_REINDEER 9
#define NUMBER_OF_ELVES 10
#define MIN_ELVES_TO_HELP 3

pthread_t reindeers[NUMBER_OF_REINDEER];
pthread_t elves[NUMBER_OF_ELVES];

pthread_mutex_t help_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t help_cond = PTHREAD_COND_INITIALIZER;

typedef struct
{
    int active_reindeers;
    int waiting_elves[MIN_ELVES_TO_HELP];
} SantaInfo;

SantaInfo santa_info = {0, {-1, -1, -1}};

bool reindeers_ready()
{
    return santa_info.active_reindeers == NUMBER_OF_REINDEER;
}

bool elves_need_help()
{
    for (int i = 0; i < MIN_ELVES_TO_HELP; i++)
    {
        if (santa_info.waiting_elves[i] == -1)
        {
            return false;
        }
    }
    return true;
}

void add_waiting_elf(int id)
{
    for (int i = 0; i < MIN_ELVES_TO_HELP; i++)
    {
        if (santa_info.waiting_elves[i] == -1)
        {
            santa_info.waiting_elves[i] = id;
            return;
        }
    }
}

void clean_waiting_elves()
{
    for (int i = 0; i < MIN_ELVES_TO_HELP; i++)
    {
        santa_info.waiting_elves[i] = -1;
    }
}

int get_number_of_waiting_elves()
{
    int counter = 0;
    for (int i = 0; i < MIN_ELVES_TO_HELP; i++)
    {
        if (santa_info.waiting_elves[i] != -1)
        {
            counter++;
        }
    }
    return counter;
}

int randint(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void santa()
{
    pthread_mutex_lock(&help_mutex);
    while (true)
    {
        puts("Mikołaj: Zasypiam");
        while (!reindeers_ready() && !elves_need_help())
        {
            pthread_cond_wait(&help_cond, &help_mutex);
        }

        if (reindeers_ready())
        {
            puts("Mikołaj: dostarcza zabawki");
            sleep(randint(2, 4));
            santa_info.active_reindeers = 0;
        }

        if (elves_need_help())
        {
            printf(
                "Mikołaj: rozwiązuje problemy elfów %d %d %d\n",
                santa_info.waiting_elves[0],
                santa_info.waiting_elves[1],
                santa_info.waiting_elves[2]);

            sleep(randint(1, 2));
            clean_waiting_elves();
        }

        pthread_cond_broadcast(&help_cond);
    }
    pthread_mutex_unlock(&help_mutex);
}

void *reindeer_routine(void *arg)
{
    int *id = (int *)arg;

    while (true)
    {
        sleep(randint(5, 10));
        pthread_mutex_lock(&help_mutex);
        santa_info.active_reindeers++;
        if (reindeers_ready())
        {
            printf("Renifer %d: wybudzam Mikołaja\n", *id);
        }
        else
        {
            printf("Renifer %d: czeka %d reniferów na Mikołaja\n", *id, santa_info.active_reindeers);
        }

        pthread_cond_broadcast(&help_cond);
        while (reindeers_ready())
        {
            pthread_cond_wait(&help_cond, &help_mutex);
        }
        pthread_mutex_unlock(&help_mutex);
    }

    return NULL;
}

void *elves_routine(void *arg)
{
    int *id = (int *)arg;

    while (true)
    {
        sleep(randint(2, 5));
        pthread_mutex_lock(&help_mutex);
        if (!elves_need_help())
        {
            add_waiting_elf(*id);
            if (elves_need_help())
            {
                printf("Elf %d: wybudzam Mikołaja\n", *id);
            }
            else
            {
                printf("Elf %d: czeka %d elfów na Mikołaja\n", *id, get_number_of_waiting_elves());
            }
            pthread_cond_signal(&help_cond);
        }
        else
        {
            printf("Elf %d: samodzielnie rozwiązuję swój problem.\n", *id);
        }
        pthread_mutex_unlock(&help_mutex);
    }

    return NULL;
}

int main()
{
    for (int i = 0; i < NUMBER_OF_REINDEER; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&reindeers[i], NULL, reindeer_routine, id);
    }

    for (int i = 0; i < NUMBER_OF_ELVES; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&elves[i], NULL, elves_routine, id);
    }

    santa();

    return 0;
}