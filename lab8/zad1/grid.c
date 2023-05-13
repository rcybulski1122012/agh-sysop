#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

const int grid_width = 20;
const int grid_height = 20;
int test = 0;

typedef struct {
    int x;
    int y;
    char* src;
    char* dst;
} RoutineInput;

pthread_t **threads;
RoutineInput **args;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void* start_routine(void* args) {
    RoutineInput *input = (RoutineInput*) args;
    while(1) {
        pause();
        input->dst[grid_width * input->y + input->x] = is_alive(input->y, input->x, input->src);

        char* tmp = input->src;
        input->src = input->dst;
        input->dst = tmp;
    }

    return NULL;
}

// used to break the pause
void dummy_handler(int signo, siginfo_t *info, void *context) {}

void init_threads(char* src, char* dst) {
    threads = (pthread_t**)malloc(grid_height * sizeof(pthread_t*));
    args = (RoutineInput **)malloc(grid_height * sizeof(RoutineInput *));

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = dummy_handler;
    sigaction(SIGUSR1, &action, NULL);

    for(int i=0; i<grid_height; i++) {
        threads[i] = (pthread_t*)malloc(grid_width * sizeof(pthread_t));
        args[i] = (RoutineInput*)malloc(grid_width * sizeof(RoutineInput));
        for(int j=0; j<grid_width; j++) {
            args[i][j].dst = dst;
            args[i][j].src = src;
            args[i][j].x = j;
            args[i][j].y = i;
        }
    }


    for(int i=0; i<grid_height; i++) {
        for(int j=0; j<grid_width; j++) {
            pthread_create(&threads[i][j], NULL, start_routine, (void*) &args[i][j]);
        }
    }
}

void exit_threads() {
    for(int i=0; i<grid_height; i++) {
        for(int j=0; j<grid_width; j++) {
            pthread_cancel(threads[i][j]);
        }
        free(threads[i]);
    }
    free(threads);

    for(int i=0; i<grid_height; i++) {
        free(args[i]);
    }

    free(args);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = dummy_handler;
    sigaction(SIGUSR1, &action, NULL);

    for(int i=0; i<grid_height; i++) {
        for(int j=0; j<grid_width; j++) {
            pthread_kill(threads[i][j], SIGUSR1);
        }
    }
}