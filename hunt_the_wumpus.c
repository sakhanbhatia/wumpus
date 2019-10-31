/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PROPERTIES 6
int wumpus_matrix[10][10][MAX_PROPERTIES];
int ai_matrix[10][10][MAX_PROPERTIES];
int g_row, g_col;
int starting_row = -1, starting_col = -1;
int
main (void)
{

  char input[5];
  printf ("Size of the board: (Ex: input '5x5' for a 5 by 5 matrix) \n");
  gets (input);
  printf ("%s\n", input);
  int len = strlen (input);
  int c = 0, d = 0, found = 0;
  char a[3] = "\0", b[3] = "\0";
  do
    {
      if (input[c] == 'x')
	{
	  found = 1;
	  c++;
	}
      else if (found == 0)
	{
	  a[c] = input[c];
	  //printf("a: %s\n",a);
	  c++;
	}
      else
	{
	  b[d] = input[c];
	  //printf("b: %s\n",b);
	  d++;
	  c++;
	}
      //printf("c: %d d: %d \n",c,d);
    }
  while (c < len);

  printf ("a: %s, b: %s\n", a, b);
  g_row = atoi (a);
  g_col = atoi (b);
  printf ("Rows = %d, Cols = %d\n", g_row, g_col);
  init_all_boards (g_row, g_col);
  display_all_boards (3);
  take_board_input ();
  display_all_boards (1);
  
  begin();
  return 0;
}

void
init_all_boards (int rows, int cols)
{
  int i, j, k;
  for (i = 0; i < rows; i++)
    {
      for (j = 0; j < cols; j++)
	{
	  for (k = 0; k < MAX_PROPERTIES; k++)
	        wumpus_matrix[i][j][k] = 0;
	  ai_matrix[i][j] = 0;
	}
    }
}

void
display_all_boards (int choice)
{
  int i, j, k;
  if (choice == 1 || choice == 3)
    {
      printf ("\nWumpus Matrix :\n");
      for (i = 0; i < g_row; i++)
	{
	  for (j = 0; j < g_col; j++)
	    {
	      for (k = 0; k < MAX_PROPERTIES; k++)
		{
		  //printf("%d",wumpus_matrix [i][j][k]);
		  if (wumpus_matrix[i][j][k] == 1)
		        convert (k);
		}
	      printf ("\t");
	    }
	  printf ("\n");
	}
    }

  if (choice == 2 || choice == 3)
    {
      printf ("AI Matrix :\n");
      for (i = 0; i < g_row; i++)
	{
	  for (j = 0; j < g_col; j++)
	    {
	      printf ("%d \t", ai_matrix[i][j]);
	    }
	  printf ("\n");
	}
    }
}

void
take_board_input ()
{
  printf
    ("Enter the property of slots: (Ex: for 0,0 to be the entry point, enter = 00E)\n");
  printf ("Enter 'exit' to stop entering\n");
  char input[10];
  while (1)
    {
      gets (input);
      if (strcmp (input, "exit") != 0)
	{
	  printf ("%s\n", input);
	  parse_and_add (input);
	}
      else
	{
	  printf ("exit inputted");
	  break;
	}
    }
}

void
parse_and_add (char *inp)
{
  //printf("Input received: %s\n", inp);
  char x[2] = "\0", y[2] = "\0";
  int len = strlen (inp);
  int i, j, index = 0;
  x[0] = inp[0];
  y[0] = inp[1];

  i = atoi (x);
  j = atoi (y);
  printf ("co-ordinates received: %d,%d \n", i, j);


  index = 2;
  do
    {
      printf ("%c\n", inp[index]);
      if (inp[index] == 'E')
	{
	  wumpus_matrix[i][j][0] = 1;
	  starting_row = i;
	  starting_col = j;
	}
      if (inp[index] == 'B')
	{
	  wumpus_matrix[i][j][1] = 1;
	}
      if (inp[index] == 'S')
	{
	  wumpus_matrix[i][j][2] = 1;
	}
      if (inp[index] == 'P')
	{
	  wumpus_matrix[i][j][3] = 1;
	}
      if (inp[index] == 'W')
	{
	  wumpus_matrix[i][j][4] = 1;
	}
      if (inp[index] == 'G')
	{
	  wumpus_matrix[i][j][5] = 1;
	}

      index++;
    }
  while (index < len);
}


void
convert (int index)
{
  switch (index)
    {
    case 0:
      printf ("E");
      break;
    case 1:
      printf ("B");
      break;
    case 2:
      printf ("S");
      break;
    case 3:
      printf ("P");
      break;
    case 4:
      printf ("W");
      break;
    case 5:
      printf ("G");
      break;
    default:
      break;
    }
}

void begin()
{
    if(starting_row != -1 && starting_col != -1)
    {
        read(starting_row,starting_col);
    }
}


void read(int row, col)
{
    int k;
    for(k=0;k<MAX_PROPERTIES;k++)
    {
        ai_matrix[row][col][k] = wumpus_matrix[row][col][k];
    }
    //action(row,col);
    
}

void action(int row, col)
{
    int k;
    for(k=0;k<MAX_PROPERTIES;k++)
    {
        if (ai_matrix[row][col][k] == 1)
        {
            
        }
    }
}