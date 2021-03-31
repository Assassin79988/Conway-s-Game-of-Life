#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "cursor.h"

unsigned int HEIGHT;
unsigned int WIDTH;
unsigned int generation = 0; 

char** grid = NULL;
char** previous_grid = NULL;
char block = '@';

// Function declarations
void initialize_grid(void);
void draw(void);
void waitFor (unsigned int);
int num_neighbours(int, int);
void compute_next_generation(void);

// Function signature
void initialize_grid(void)
{
	// seeds the rand() function
	srand((unsigned) time(NULL));
	int randnum;
	grid = (char**)malloc(HEIGHT * sizeof(char*));
	for(int i = 0; i < HEIGHT; ++i) {grid[i] = (char*)malloc(WIDTH*sizeof(char));}
	// Initializes a grid with each being randomly chosen as being alive (= block) or dead(= ' ')
	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			randnum = rand()%2;
			if(randnum != 0)
			{
				grid[i][j] = ' ';
			}
			else
			{
				grid[i][j] = block;
			}
		}
	}
}

int num_neighbours(int x, int y)
{
	int neighbours = 0;
	// finds all alive neighbours of grid[x][y]
	for(int i = -1; i < 2; ++i)
	{
		for(int j = -1; j < 2; ++j)
		{
			int col = (x + i + HEIGHT) % HEIGHT;
			int row = (y + j + WIDTH) % WIDTH;
			if(grid[col][row] == block) {neighbours += 1;}
		}
	}
	if(grid[x][y] == block){neighbours -= 1;}
	return neighbours;	
}

void draw(void)
{
	// prints out the current generation
	cursor_goto_pos(HEIGHT+2,0);
	printf("Generation:%d", generation);
	// prints the new cell states
	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			if(generation == 0)
			{
                                cursor_goto_pos(i+1,j+1);
				printf("%c",grid[i][j]);
				
			}
			else if(grid[i][j] != previous_grid[i][j])
			{
				cursor_goto_pos(i+1,j+1);
				printf("%c",grid[i][j]);	
			}
		}
	}
}

void waitFor (unsigned int millisecs)
{
    // simple delay function
    long pause;
    clock_t now,then;
    pause = millisecs*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
    {
    	now = clock();    
    }
}
void compute_next_generation(void)
{
	int neighbours;
	char** new_grid = (char**)malloc(HEIGHT * sizeof(char*));
	for(int i = 0; i < HEIGHT; ++i) {new_grid[i] = (char*)malloc(WIDTH*sizeof(char));}
	if(previous_grid != NULL) { free(previous_grid);}
	previous_grid = grid;
	// find the new states for each grid tile.
	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			neighbours = num_neighbours(i, j);
			if(grid[i][j] == block && (neighbours < 2 || neighbours > 3)) {new_grid[i][j] = ' ';}
			else if(grid[i][j] == ' ' && neighbours == 3) {new_grid[i][j] = block;}
			else{new_grid[i][j] = grid[i][j];}
		}
	}

	generation += 1;
	grid = new_grid;
}

int main(void)
{
	cursor_clear_screen();
	HEIGHT = get_height() - 2;
	WIDTH = get_width();
	initialize_grid();
	draw();
	while(1)
	{
		waitFor(100);
		compute_next_generation();
		draw();
		
	}
	return 0; 
}
