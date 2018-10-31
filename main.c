/* Conway's game of life */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

int8_t advance(int64_t * universe, int64_t * parallel_universe, int32_t n);

int main(void) {
    /* Set n */
    int32_t n = 50;

    /* the universe is a square, there is also a parallel universe */
    int64_t universe[n * n], parallel_universe[n * n];

    /* fill universe with 1s and 0s */
    srand(time(NULL));
    for (int64_t i = 0; i < (n * n); ++i) {
        universe[i] = (rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1);
    }

    /* start curses */
    initscr();
    clear();
    refresh();

    /* the game of life */
    while(advance(universe, parallel_universe, n))
        ;

    /* this part will never execute but it's here for good measure */
    endwin();
    return 0;
}

int8_t advance(int64_t * universe, int64_t * parallel_universe, int32_t n) {
    int8_t neighbors[8];

    /*
     * neighbors
     *
     * 0  1  2
     * 3  X  4
     * 5  6  7
     *
     */

    /* Calculate next iteration into the parallel universe */
    for (int64_t i = 0; i < (n * n); ++i) {

        /* Print universe as it is */
        for (int64_t j = 0; j < (n * n); ++j) {
            mvprintw(j / n, 2 * (j % n), (universe[j] ? "*" : " "));
        }
        refresh();

        /* See how the neighbors are */
        neighbors[0] = (i <= n) ? 0 : ((i % 10 == 0) ? 0 : universe[i - 11]);
        neighbors[1] = (i <= n) ? 0 : universe[i - 10];
        neighbors[2] = (i <= n) ? 0 : ((i % 9 == 0) ? 0 : universe[i - 9]);
        neighbors[3] = (i % 10 == 0) ? 0 : universe[i - 1];
        neighbors[4] = (i % 9 == 0) ? 0 : universe[i + 1];
        neighbors[5] = (i >= ((n * n) - n)) ? 0 : ((i % 10 == 0) ? 0 : universe[i + 9]);
        neighbors[6] = (i >= ((n * n) - n)) ? 0 : universe[i + 10];
        neighbors[7] = (i >= ((n * n) - n)) ? 0 : ((i % 9 == 0) ? 0 : universe[i + 11]);

        /* Count active neighbors */
        int8_t active_neighbors = 0;
        for (int8_t j = 0; j < 8; ++j) {
            active_neighbors += neighbors[j];
        }

        /* Calculate i in parallel_universe*/
        switch (universe[i]) {
            case 0:
                switch (active_neighbors) {
                    case 0:
                    case 1:
                        parallel_universe[i] = 0;
                        break;
                    case 2:
                    case 3:
                        parallel_universe[i] = 1;
                        break;
                    default:
                        parallel_universe[i] = 0;
                        break;
                }
                break;
            case 1:
                switch (active_neighbors) {
                    case 3:
                        parallel_universe[i] = 1;
                        break;
                    default:
                        parallel_universe[i] = 0;
                        break;
                }
                break;
            default:
                break;
        }
    }

    /* Copy parellel universe into the universe */
    for (int64_t j = 0; j < (n * n); ++j) {
        universe[j] = parallel_universe[j];
    }

    /* Wait two seconds */
    sleep(1);

    return 1;
}
