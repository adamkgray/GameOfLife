/* Conway's game of life */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

int32_t advance(int8_t * universe, int8_t * parallel_universe, int16_t n, int16_t ms);

int main(int argc, char ** argv) {
    /* Start curses */
    initscr();
    curs_set(0);
    clear();
    refresh();

    /* Dimensions */
    int16_t n = LINES - 2;
    /* Milliseconds */
    int16_t ms = 1000;

    /* Handle command line args */
    if (argc >= 2) {
        for (int8_t i = 1; i < argc; ++i) {

            /* Dimensions */
            if (!strcmp(argv[i], "-n")) {
                if (i + 1 == argc) {
                    endwin();
                    fprintf(stderr, "gameoflife\n\t-n\tinteger grid dimensions\n\t-m\tmilliseconds between each step\n");
                    return 1;
                }
                n = atoi(argv[i + 1]);
                if (n <= 0) {
                    endwin();
                    fprintf(stderr, "gameoflife\n\t-n\tinteger grid dimensions\n\t-m\tmilliseconds between each step\n");
                    return 1;
                } else {
                    ++i;
                }

            /* Milliseconds */
            } else if (!strcmp(argv[i], "-m")) {
                if (i + 1 == argc) {
                    endwin();
                    fprintf(stderr, "gameoflife\n\t-n\tinteger grid dimensions\n\t-m\tmilliseconds between each step\n");
                    return 1;
                }
                ms = atoi(argv[i + 1]);
                if (ms <= 0) {
                    endwin();
                    fprintf(stderr, "gameoflife\n\t-n\tinteger grid dimensions\n\t-m\tmilliseconds between each step\n");
                    return 1;
                } else {
                    ++i;
                }
            } else {
                endwin();
                fprintf(stderr, "gameoflife\n\t-n\tinteger grid dimensions\n\t-m\tmilliseconds between each step\n");
                return 1;
            }
        }
    }


    /* The universe is a square */
    int8_t * universe = (int8_t *)malloc((n * n) * sizeof(int8_t));
    if (universe == NULL) {
        endwin();
        return 1;
    }

    /* Parallel universe holds next version of universe during calculations */
    int8_t * parallel_universe = (int8_t *)malloc((n * n) * sizeof(int8_t));
    if (parallel_universe == NULL) {
        endwin();
        return 1;
    }

    /* Fill universe with 1s and 0s */
    srand(time(NULL));
    for (int32_t i = 0; i < (n * n); ++i) {
        universe[i] = rand() & 1;
    }

    /* The game of life */
    while(advance(universe, parallel_universe, n, ms))
        ;

    /* Program ends when all cells are inactive */
    endwin();
    return 0;
}

int32_t advance(int8_t * universe, int8_t * parallel_universe, int16_t n, int16_t ms) {
    /* Keep track of how many steps have been taken so far */
    static int16_t steps = 0;

    /* Number of active cells */
    int32_t active = 0;

    for (int32_t i = 0; i < (n * n); ++i) {
        /* Increment active */
        active += universe[i];

        /* Add universe cell to screen */
        mvaddch(i / n, 2 * (i % n), (universe[i] ? ACS_CKBOARD : ' '));

        /* Count active neighbors */
        int8_t active_neighbors =
        ((i < n) ? 0 : (i % n == 0) ? 0 : universe[i - n - 1])                         /* 0 */
        + ((i < n) ? 0 : universe[i - n])                                              /* 1 */
        + ((i < n) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i - n + 1])                 /* 2 */
        + ((i % n == 0) ? 0 : universe[i - 1])                                         /* 3 */
        + (((i + 1) % n == 0) ? 0 : universe[i + 1])                                   /* 4 */
        + ((i >= ((n * n) - n)) ? 0 : (i % n == 0) ? 0 : universe[i + n - 1])          /* 5 */
        + ((i >= ((n * n) - n)) ? 0 : universe[i + n])                                 /* 6 */
        + ((i >= ((n * n) - n)) ? 0 : ((i + 1) % n == 0) ? 0 : universe[i + n + 1]);   /* 7 */

        /* Calculate i in parallel_universe*/
        switch (universe[i]) {
            case 1:                                 /* Active cell */
                switch (active_neighbors) {
                    case 0:
                    case 1:
                        parallel_universe[i] = 0;   /* Too few neigbors */
                        break;
                    case 2:
                    case 3:
                        parallel_universe[i] = 1;   /* Right amount of neighbors */
                        break;
                    default:
                        parallel_universe[i] = 0;   /* Too many neighbors */
                        break;
                }
                break;
            case 0:                                 /* Inactive cell */
                switch (active_neighbors) {
                    case 3:                         /* Right amount of neighbors */
                        parallel_universe[i] = 1;
                        break;
                    default:                        /* Too few or too many neighbors */
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
        *(universe++) = *(parallel_universe++);
    }

    /* Wait one second */

    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);

    /* If active is 0, program will exit */
    return active;
}
