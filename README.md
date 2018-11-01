# Game of Life

Terminal version of Conway's 'Game of Life'

To compile, clone the repo and then run `make`

This will produce an executable called `gameoflife`

`-n <int>` specifies the dimensions of the grid

`-m <int>` specifies the milliseconds between each step

Example:

`./gameoflife -n 20 -m 100`

Otherwise, `gameoflife` defaults to a gridsize that it 2 less than the terminal height, and steps that are 1000 milliseconds

