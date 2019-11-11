/* HUNT THE WUMPUS
/* You are to build a Wumpus World Solver. The solver will take a description of world as follows:
/*
/* Size = RxC (R= number of rows, C = number of columns)
/*
/* Attributes:
/* RCX (X = Attribute of the slot at (R,C))
/*      where:
/*          W = Wumpus
/*          B = Breezy
/*          S = Stench
/*          G = Gold
/*          P = Pit
/*          E = Entry(Start Position)
/*
/*
/*
/*  From this you will build a solver that uses a Logical Agent to solve the problem.
/*  For test use the following description:
/*  Size : 5x5
/*  00E
/*  01B
/*  02P
/*  03B
/*  12B
/*  20B
/*  22S
/*  24B
/*  30P
/*  31BS
/*  32W
/*  33GBS
/*  34P
/*  40B
/*  41P
/*  42BS
/*  44B
/*  exit
/*
/*  If your agent lands in a Pit or lands on the Wumpus square the game is over. Try to get the Gold without waking the Wumpus
/*  and return back to your entry point.
/*  You can kill the Wumpus if you know the exact location of it.
/*
/*  Scoring:
/*  Each moves costs one point.
/*  Killing reduces points by 10.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#define MAX_ROW 10
#define MAX_COL 10
#define WM_MAX_PROPERTIES 6
#define AIM_MAX_PROPERTIES 10
#define ENTRY 0
#define BREEZY 1
#define STENCH 2
#define PIT 3
#define WUMPUS 4
#define GOLD 5
#define PIT_WEIGHT 6
#define WUMPUS_WEIGHT 7
#define VISITED 8
#define SAFE 9
#define LIMIT_MAX_MOVES 30
#define START_SCORE 1000
#define INPUT_FILE 1

int wumpus_matrix[MAX_ROW][MAX_COL][WM_MAX_PROPERTIES];
int ai_matrix[MAX_ROW][MAX_COL][AIM_MAX_PROPERTIES];
int g_row, g_col;
int starting_row = -1, starting_col = -1;
int current_row = 0, current_col = 0;
int possible_moves[MAX_ROW*MAX_COL][2];
int possible_moves_count =0;
int actual_moves[MAX_ROW*MAX_COL][2];
int actual_moves_pointer = 0;
int total_moves =0;
int duplicate_counter=0;

int wumpus_row = -1, wumpus_col =-1;
int wumpus_found = 0;
int wumpus_killed = 0;

main (void)
{

  char input[5];
  printf ("Size of the board: (Ex: input '5x5' for a 5 by 5 matrix) \n");
  gets (input);
  //printf ("%s\n", input); /*print on*/
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

  //printf ("a: %s, b: %s\n", a, b); /*print on*/
  g_row = atoi (a);
  g_col = atoi (b);
  //printf ("Rows = %d, Cols = %d\n", g_row, g_col); /*print on*/
  init_all_boards (g_row, g_col);
  //display_all_boards (3); /*print on*/
  if( INPUT_FILE == 0)
  {
    take_board_input ();
  }
  else
  {
    take_file_input();
  }


  display_all_boards (1);

  begin();
  printf("\nWUMPUS WORLD SOLVED!\n");
  //display_all_boards (3);/*print on*/
  return 0;
}

void
init_all_boards (int rows, int cols)
{
  int i, j, k, l;
  for (i = 0; i < rows; i++)
    {
      for (j = 0; j < cols; j++)
	{
	  for (k = 0; k < WM_MAX_PROPERTIES; k++)
        wumpus_matrix[i][j][k] = 0;
      for(l=0;l<AIM_MAX_PROPERTIES;l++)
        ai_matrix[i][j][l] = 0;
	}
    }
}

void
display_all_boards (int choice)
{
  int i, j, k, l;
  if (choice == 1 || choice == 3)
    {
      printf ("Wumpus Matrix :\n");
      for (i = 0; i < g_row; i++)
	{
	  for (j = 0; j < g_col; j++)
	    {
	        if(i== current_row && j== current_col)
                printf("*");
	      for (k = 0; k < WM_MAX_PROPERTIES; k++)
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
	        for(l=0;l<AIM_MAX_PROPERTIES;l++)
            {
                printf ("%d", ai_matrix[i][j][l]);
            }
	      printf("\t");
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
	  //printf ("%s\n", input); /*print on*/
	  parse_and_add (input);
	}
      else
	{
	  printf ("exit inputted");
	  break;
	}
    }
}

void take_file_input()
{
    FILE *fp;
    fp = NULL;
    char data[10];
    if(INPUT_FILE == 1)
        fp = fopen("wumpus_world.txt","r");
    else if (INPUT_FILE == 2)
        fp = fopen("wumpus_world1.txt","r");
    else if (INPUT_FILE == 3)
        fp = fopen("wumpus_world2.txt","r");

    if (fp == NULL)
    {
        printf("Couldn't open file specified\n");
        exit(0);
    }
    printf("Taking input from file\n");
    while(fgets(data, 10, fp) != NULL)
    {
        if (strcmp (data, "exit") != 0)
        {
            //printf ("%s\n", data); /*print on*/
            parse_and_add (data);
        }
        else
        {
            //printf ("exit inputted\n");/*print on*/
            break;
        }
    }
    //printf("Closing file\n");/*print on*/
    fclose(fp);
    return;
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
  //printf ("co-ordinates received: %d,%d \n", i, j);/*print on*/


  index = 2;
  do
    {
      //printf ("%c\n", inp[index]); /*print on*/
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
        current_row = starting_row;
        current_col = starting_col;
        init_moves_list();
        read(starting_row,starting_col);
    }
    else
    {
        printf("Entry Point missing!\n");
        return;
    }
}


void read(int row, int col)
{
    int k;
    for(k=0;k<WM_MAX_PROPERTIES;k++)
    {
        ai_matrix[row][col][k] = wumpus_matrix[row][col][k];
    }

    if(ai_matrix[row][col][PIT] == 1)
    {
        printf("You died because you fell into a pit at %d,%d.\n",row,col);
        ai_matrix[row][col][VISITED] = 1;
        exit(0);
    }
    if(ai_matrix[row][col][WUMPUS] == 1)
    {
        if(wumpus_killed == 1)
        {
            ai_matrix[row][col][WUMPUS]==0;
        }
        else
        {
            printf("You dies because you woke the Wumpus\n");
            ai_matrix[row][col][VISITED] = 1;
            exit(0);
        }
    }
    if(ai_matrix[row][col][GOLD] == 1)
    {
        printf("\nReached GOLD at %d,%d!!!\n",row,col);
        printf("\n\nTotal Moves: %d\nWumpus Killed : %s\nMoves back to start: %d\n"
               "Score: %d\n",total_moves,wumpus_killed?"Yes":"No",actual_moves_pointer,(START_SCORE - (total_moves+actual_moves_pointer+(wumpus_killed*10))));
        return;
    }


    action(row,col,0); //Take appropriate actions on the new information.
    locate_wumpus();
    //display_all_boards(2); /*print on*/
    int next_move_index = decide_next_move(row,col);
    if(next_move_index != -1)
    {
        int valid_move = check_move(next_move_index);
        if(valid_move)
        {
            move(next_move_index);
        }
        else
        {
            printf("Encountered an invalid move\nCurrent Position: %d,%d.\nNext Move decided: %d,%d\n",current_row,current_col, possible_moves[next_move_index][0],possible_moves[next_move_index][1]);
            exit(0);
        }
    }
    else // For backtracked move
    {
        //printf("Backtracked move, skipped move and calling read\n");/*print on*/
        //exit(0);
        read(current_row, current_col);
    }
    return;
     //Handle this. It should not exit, but try to find a valid move
}

void action(int row, int col, int mark_for_visited)
{
    if(duplicate_counter>=2)
    {
        kill_wumpus();
    }
    if((ai_matrix[row][col][VISITED] == 1) && (mark_for_visited == 0))
        return;
    if ((ai_matrix[row][col][BREEZY] == 1) && (mark_for_visited == 0))
        mark_adj(row,col,BREEZY);
    if(ai_matrix[row][col][STENCH] == 1)
        mark_adj(row,col,STENCH);
    //if(ai_matrix[row][col][GOLD] == 1)
        //FILL action;
    if ((ai_matrix[row][col][BREEZY] == 0) && ((ai_matrix[row][col][STENCH] == 0) || (wumpus_found == 1)))
        mark_adj(row,col,SAFE);

    ai_matrix[row][col][VISITED] = 1;
}

void mark_adj(int row,int col,int property)
{
    //We have to mark the adjacent slots
    if((row-1)>=0)
    {
        mark_adj_slots(row-1,col,property);
    }
    if((row+1)<g_row)
    {
        mark_adj_slots(row+1,col,property);
    }
    if((col-1)>=0)
    {
        mark_adj_slots(row,col-1,property);
    }
    if((col+1)<g_col)
    {
        mark_adj_slots(row,col+1,property);
    }
    return;
}


void mark_adj_slots(int r, int c, int p)
{
    if(p == BREEZY)
        ai_matrix[r][c][PIT_WEIGHT]++;
    else if((wumpus_found==0) && (p == STENCH))
        ai_matrix[r][c][WUMPUS_WEIGHT]++;
    else if((wumpus_found==1) && (p==STENCH))
    {
        if((ai_matrix[r][c][PIT_WEIGHT]==0) && (ai_matrix[r][c][PIT]==0))
            ai_matrix[r][c][SAFE]=1;
    }
    else if(p == SAFE)
    {
        ai_matrix[r][c][SAFE] = 1;
        ai_matrix[r][c][PIT_WEIGHT] == 0;
        ai_matrix[r][c][WUMPUS_WEIGHT] == 0;
    }

}

int decide_next_move(row,col)
{
    get_possible_moves(row,col);
    int visited = 0;
    int p_row= -1, p_col= -1;
    int index_pointer = possible_moves_count-1;
    do
    {
        p_row = possible_moves[index_pointer][0];
        p_col = possible_moves[index_pointer][1];
        if(ai_matrix[p_row][p_col][VISITED] == 1)
        {
            visited++;
            index_pointer--;
            continue;
        }
        if(ai_matrix[p_row][p_col][WUMPUS] == 1)
        {
            index_pointer--;
            continue;
        }
        if(ai_matrix[p_row][p_col][SAFE] == 1)
        {
            //printf("Returning %d index\n",index_pointer);/*print on*/
            return (index_pointer);
        }

        index_pointer--;
    }while(index_pointer>=0);

    if(visited >1)
    {
        index_pointer = possible_moves_count-1;
        int count =0;
        int random_visited_node = (rand() % visited);
        //printf("Random Visited node to be selected is = %d\n", random_visited_node);/*print on*/
        do
        {
            p_row = possible_moves[index_pointer][0];
            p_col = possible_moves[index_pointer][1];
            if(ai_matrix[p_row][p_col][VISITED] == 1)
            {
                if (count == random_visited_node)
                {
                    return(index_pointer);
                }
                count++;
            }
            index_pointer--;
        }while(index_pointer>=0);
    }

    if (index_pointer <0)
    {
        //printf("go back one step\n");/*print on*/
        backtrack();
        return -1;
    }
}

void init_moves_list()
{
    int i,j;
    for(i=0;i<(g_row*g_col);i++)
    {
        for(j=0;j<2;j++)
        {
            possible_moves[i][j] = 0;
            actual_moves[i][j] = 0;
        }
    }
    actual_moves[0][0] = starting_row;
    actual_moves[0][1] = starting_col;
}


void get_possible_moves(int r, int c)
{
    possible_moves_count = 0;
    if((c-1)>=0)
    {
        possible_moves[possible_moves_count][0] = r;
        possible_moves[possible_moves_count][1] = c-1;
        possible_moves_count++;
    }
    if((r-1)>=0)
    {
        possible_moves[possible_moves_count][0] = r-1;
        possible_moves[possible_moves_count][1] = c;
        possible_moves_count++;
    }
    if((c+1)<g_col)
    {
        possible_moves[possible_moves_count][0] = r;
        possible_moves[possible_moves_count][1] = c+1;
        possible_moves_count++;
    }
    if((r+1)<g_row)
    {
        possible_moves[possible_moves_count][0] = r+1;
        possible_moves[possible_moves_count][1] = c;
        possible_moves_count++;
    }
    //To display possible moves after each call: (comment later)
    //printf("Possible moves :\n");/*print on*/
    /*for(int i=0;i<possible_moves_count;i++) /*print on*/
    /*{
        printf("%d, %d\n",possible_moves[i][0],possible_moves[i][1]);
    }
    printf("total Possible moves now: %d\n",possible_moves_count);*//*print on*/
    return;

}

int check_move(int indx)
{
    int pt_row,pt_col;
    pt_row = possible_moves[indx][0];
    pt_col = possible_moves[indx][1];
    //printf("Check move at Index: %d",indx);/*print on*/
    //printf("Values %d,%d and %d,%d\n",current_row,current_col, pt_row, pt_col); /*print on*/
    if((pt_row == current_row) && (pt_col == current_col))
    {
        //printf("Row and Col are equal");/*print on*/
        return 0;
    }

    else if((((pt_row == current_row-1) || (pt_row == current_row+1)) && (pt_col == current_col))
            || (((pt_col == current_col-1) || (pt_col == current_col+1)) && (pt_row == current_row)))
    {
        //printf("Row and Col are adjacent\n");/*print on*/
        return 1;
    }

    else
        return 0;
}

void move(int index)
{
    int n_row,n_col;

    n_row = possible_moves[index][0];
    n_col = possible_moves[index][1];

    current_row = n_row;
    current_col = n_col;

    actual_moves_pointer++;
    total_moves++;
    printf("\n\nMove: %d\n",total_moves);

    actual_moves[actual_moves_pointer][0] = current_row;
    actual_moves[actual_moves_pointer][1] = current_col;

    display_all_boards(1);
    printf("Moves from start:\n");
    for(int i=0;i<=actual_moves_pointer;i++)
    {
        printf("(%d,%d)",actual_moves[i][0],actual_moves[i][1]);
    }
    printf("\n");

    if(total_moves >= LIMIT_MAX_MOVES)
    {
        printf("Reached max moves limit: %d\n", LIMIT_MAX_MOVES);
        exit(0);
    }
    check_duplicate_moves();
    read(current_row,current_col);
    return;
}

void backtrack()
{
    int next_row, next_col;
    next_row = actual_moves[actual_moves_pointer-1][0];
    next_col = actual_moves[actual_moves_pointer-1][1];
    //printf("Back track move to %d,%d\n",next_row, next_col);/*print on*/
    current_row = next_row;
    current_col = next_col;

    total_moves++;
    actual_moves_pointer--;
    printf("Move: %d\n",total_moves);

    display_all_boards(1);

    printf("Moves from start:\n");
    for(int i=0;i<=actual_moves_pointer;i++)
    {
        printf("(%d,%d)",actual_moves[i][0],actual_moves[i][1]);
    }
    printf("\n");
    if (total_moves >= LIMIT_MAX_MOVES)
    {
        printf("Reached max moves limit: %d\n", LIMIT_MAX_MOVES);
        exit(0);
    }
    return;
    //printf("Exiting\n");
    //exit(0);
}


void locate_wumpus()
{
    int i,j;
     for (i = 0; i < g_row; i++)
	{
	  for (j = 0; j < g_col; j++)
	    {
	        if((ai_matrix[i][j][WUMPUS_WEIGHT] >=2) && (ai_matrix[i][j][SAFE] == 0))
            {
               wumpus_row = i;
               wumpus_col = j;
               wumpus_found = 1;
               ai_matrix[i][j][WUMPUS] = 1;
               printf("WUMPUS LOCATED at %d,%d\n",wumpus_row, wumpus_col);
               disregard_stench();
            }
	    }
	}
	return;
}

void disregard_stench()
{
    int i,j;
    for (i = 0; i < g_row; i++)
	{
	  for (j = 0; j < g_col; j++)
	    {
            ai_matrix[i][j][WUMPUS_WEIGHT] = 0;
	    }
	}
	for(i=0;i<g_row;i++)
    {
        for(j=0;j<g_col;j++)
        {
            if(ai_matrix[i][j][VISITED] == 1)
            {
                action(i,j,1);
            }
        }
    }
	return;
}


void kill_wumpus()
{
    if(wumpus_found == 0)
    {
        printf("Can't kill wumpus as wumpus not found\n");
        return;
    }
    else if (wumpus_killed == 0)
    {
        ai_matrix[wumpus_row][wumpus_col][WUMPUS]=0;
        wumpus_killed=1;
        ai_matrix[wumpus_row][wumpus_col][SAFE] = 1;
        action(wumpus_row,wumpus_col,0);
        ai_matrix[wumpus_row][wumpus_col][VISITED]=0;

        printf("KILLED WUMPUS at %d,%d\n",wumpus_row,wumpus_col);
    }
    return;
}

void check_duplicate_moves()
{
    int i,j,k;
    int row_value, col_value;
    for(i=0;i<=actual_moves_pointer;i++)
    {
        row_value=actual_moves[i][0];
        col_value=actual_moves[i][1];
        for(j=i+1;j<=actual_moves_pointer;j++)
        {
            if((row_value == actual_moves[j][0]) && (col_value == actual_moves[j][1]))
            {
                duplicate_counter++;
                for(k=i+1;k<=actual_moves_pointer;k++)
                {
                    actual_moves[k][0]=-1;
                    actual_moves[k][1]=-1;
                }
                actual_moves_pointer=i;
                //printf("Duplicate route removed\n");/*print on*/
                //printf("Duplicate Counter: %d\n",duplicate_counter);/*print on*/
            }
        }
    }
    return;
}
