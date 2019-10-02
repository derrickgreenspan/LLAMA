/*
    Copyright (C) 2018, Derrick Greenspan and the University of Central Florida

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
    Email: Derrick.Greenspan@knights.ucf.edu
    Website: https://sites.google.com/site/derrickgreenspan/
*/


/* MATRIX MULTIPLICATION - DYNAMIC MEMORY EDITION 
 * (callocs) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "library/llama.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b)) 

struct matrix *multiply(struct matrix a, struct matrix b);
char manual_mode; 
/*
struct matrix
{
	int *value;
	int row_size;
	int column_size;
};
*/

//void ariel_enable();
int main(int argc, char **argv)
{

	//FILE *output = stdout;
	ariel_enable();
//	struct matrix *a,*b;
	int *a, *b;
	int col1 = atoi(argv[1]), row1 = atoi(argv[2]);
	int col2 = atoi(argv[3]), row2 = atoi(argv[4]);

	a = calloc(sizeof(int **), row1 * col1);
	b = calloc(sizeof(int **), row2 * col2);

	int i, j;
	i = 0;
	while(i < row1)
	{
		j = 0;
		while(j < col1)
		{
			a[i + col1 * j] = (j + i) % 10;
			j++;
		}
		i++;
	}

	i = 0;
	while(i < row2)
	{
		j = 0;
		while(j < col2)
		{
			b[i + col2 * j] = (j + i) % 10;
			j++;
		}
		i++;
	}



	int *c;
	int row3, col3;
	row3 = min(row1, row2);
	col3 = min(col1, col2);
	c = calloc(sizeof(int **), col3 * row3);
	int curr_sum = 0, k;

	for(i= 0; i < row1; i++)
	{
		for(j = 0; j < col2; j++)
		{
			k = 0;
			for(k = 0; k < row2; k++)
			{
				curr_sum += a[i  + row2 * k] * b[k + row2 * j];
			}
			c[i + j*col2] = curr_sum;
			curr_sum = 0;
		}
	}

	//struct matrix * c = multiply(*a, *b);
	return 0;
}



