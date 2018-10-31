/* Conway's game of life */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

int8_t advance(int64_t * universe, int64_t * parallel_universe, int32_t n);

int main(int argc, char ** argv) {
    /* Set n */
    int32_t n;
    if (argc < 2) {
        n = 10;
    } else {
        n = (int32_t)atoi(argv[1]);
    }

    /* the universe is a square, there is also a parallel universe */
    int64_t universe[n * n], parallel_universe[n * n];

    /* fill universe with 1s and 0s */
    srand(time(NULL));
    for (int64_t i = 0; i < (n * n); ++i) {
        universe[i] = (rand() & 1);
    }

    /* start curses */
    initscr();
    curs_set(0);
    clear();
    refresh();

    /* the game of life */
    while(advance(universe, parallel_universe, n))
        ;

    /* Program ends when all cells are inactive */
    endwin();
    return 0;
}

int8_t advance(int64_t * universe, int64_t * parallel_universe, int32_t n) {
    /* keep track of how many steps have been taken so far */
    static int16_t steps = 0;

    /* The sum of the universe. A sum of 0 (i.e. no active cells) will cause the program to end */
    int32_t sum = 0;

    for (int64_t i = 0; i < (n * n); ++i) {
        /* increment sum */
        sum += universe[i];

        /* Add universe cell to screen */
        mvaddch(i / n, 2 * (i % n), (universe[i] ? ACS_CKBOARD : ' '));

        /* Count active neighbors */
        int8_t active_neighbors = 0;
        for (int8_t j = 0; j < 8; ++j) {
            switch (j) {
                case 0:
                    active_neighbors += (i < n) ? 0 : (i % n == 0) ? 0 : universe[i - n - 1];
                    break;
                case 1:
                    active_neighbors += (i < n) ? 0 : universe[i - n];
                    break;
                case 2:
                    active_neighbors += (i < n) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i - n + 1];
                    break;
                case 3:
                    active_neighbors += (i % n == 0) ? 0 : universe[i - 1];
                    break;
                case 4:
                    active_neighbors += ((i + 1) % n == 0) ? 0 : universe[i + 1];
                    break;
                case 5:
                    active_neighbors += (i >= ((n * n) - n)) ? 0 : (i % n == 0) ? 0 : universe[i + n - 1];
                    break;
                case 6:
                    active_neighbors += (i >= ((n * n) - n)) ? 0 : universe[i + n];
                    break;
                case 7:
                    active_neighbors += (i >= ((n * n) - n)) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i + n + 1];
                    break;
                default:
                    break;
            }
        }

        /* Calculate i in parallel_universe*/
        switch (universe[i]) {
            case 1:                                 /* active cell */
                switch (active_neighbors) {
                    case 0:
                    case 1:
                        parallel_universe[i] = 0;   /* too few neigbors */
                        break;
                    case 2:
                    case 3:
                        parallel_universe[i] = 1;   /* right amount of neighbors */
                        break;
                    default:
                        parallel_universe[i] = 0;   /* too many neighbors */
                        break;
                }
                break;
            case 0:                                 /* inactive cell */
                switch (active_neighbors) {
                    case 3:                         /* right amount of neighbors */
                        parallel_universe[i] = 1;
                        break;
                    default:                        /* too few or too many neighbors */
                        parallel_universe[i] = 0;
                        break;
                }
                break;
            default:
                break;
        }
    }

    mvprintw(n + 1, 0, "steps: %d", steps++);

    /* Update screen */
    refresh();

    /* Copy parellel universe into the universe */
    for (int64_t j = 0; j < (n * n); ++j) {
        universe[j] = parallel_universe[j];
    }

    /* Wait one second */
    sleep(1);

    /* If sum is 0, program will exit */
    return sum;
}
