/* Conway's game of life */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

int32_t advance(int8_t * universe, int8_t * parallel_universe, int16_t n);

int main(int argc, char ** argv) {
    /* start curses */
    initscr();
    curs_set(0);
    clear();
    refresh();

    /* Set n */
    int16_t n = 0;
    if (argc >= 2) {
        n = (int16_t)atoi(argv[1]);
    }
    if (!n) {
        n = LINES - 2;  /* fill up screen height by default */
    }

    /* the universe is a square, there is also a parallel universe */
    int8_t * universe = (int8_t *)malloc((n * n) * sizeof(int8_t));
    int8_t * parallel_universe = (int8_t *)malloc((n * n) * sizeof(int8_t));

    /* fill universe with 1s and 0s */
    srand(time(NULL));
    for (int32_t i = 0; i < (n * n); ++i) {
        universe[i] = rand() & 1;
    }

    /* the game of life */
    while(advance(universe, parallel_universe, n))
        ;

    /* Program ends when all cells are inactive */
    endwin();
    return 0;
}

int32_t advance(int8_t * universe, int8_t * parallel_universe, int16_t n) {
    /* keep track of how many steps have been taken so far */
    static int16_t steps = 0;

    int32_t active = 0;

    for (int32_t i = 0; i < (n * n); ++i) {
        /* increment active */
        active += universe[i];

        /* Add universe cell to screen */
        mvaddch(i / n, 2 * (i % n), (universe[i] ? ACS_CKBOARD : ' '));

        /* Count active neighbors */
        int8_t active_neighbors = 0;
        active_neighbors += (i < n) ? 0 : (i % n == 0) ? 0 : universe[i - n - 1];                       /* 0 */
        active_neighbors += (i < n) ? 0 : universe[i - n];                                              /* 1 */
        active_neighbors += (i < n) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i - n + 1];                 /* 2 */
        active_neighbors += (i % n == 0) ? 0 : universe[i - 1];                                         /* 3 */
        active_neighbors += ((i + 1) % n == 0) ? 0 : universe[i + 1];                                   /* 4 */
        active_neighbors += (i >= ((n * n) - n)) ? 0 : (i % n == 0) ? 0 : universe[i + n - 1];          /* 5 */
        active_neighbors += (i >= ((n * n) - n)) ? 0 : universe[i + n];                                 /* 6 */
        active_neighbors += (i >= ((n * n) - n)) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i + n + 1];    /* 7 */

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
                return 0;
                break;
        }
    }

    mvprintw(n + 1, 0, "steps: %d | active: %.5ld", steps++, active);

    /* Update screen */
    refresh();

    /* Copy parellel universe into the universe */
    for (int32_t i = 0; i < (n * n); ++i) {
        universe[i] = parallel_universe[i];
    }

    /* Wait one second */
    sleep(1);

    /* If active is 0, program will exit */
    return active;
}
