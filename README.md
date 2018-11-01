# Game of Life

Terminal version of Conway's 'Game of Life'

To compile: `gcc main.c -l ncurses`

The program defaults to a grid of height and width two less than the height of the terminal and 1000 milliseconds between each step

The program can take two optional flags
* `-n` followed by an integer resizes the dimensions of the grid
* `-m` followed by an integer specifies the number of milliseconds between each step 

Example:

```
gameoflife -n 20 -m 100
```

This specifies a 10x10 grid and 100 ms between each step
