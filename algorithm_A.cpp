#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <random>
#include "../include/algorithm.h"
#include "../include/Position.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/


void update_cell_value(Board,Player,int (&) [5][6]);
bool put_corner(Board,Player);
Position get_corner_coordinate(Board,Player);
bool put_edge(Board,Player);
Position get_edge_coordinate(Board,Player);



void algorithm_A(Board board, Player player, int index[]){

    int cell_value[5][6]={0};
    mt19937 mt_rand(time(NULL));
    update_cell_value(board,player,cell_value);
    Position coordinate;
    int value = 0;
    int color = player.get_color();
    int enemy_color = (color=='r')?'b':'r';
    for (int i=0;i<5;i++)
    {
        for (int j=0;j<6;j++)
        {
            if (cell_value[i][j] > value)
            {
                value = cell_value[i][j];
                coordinate.first = i;
                coordinate.second = j;
            }
        }
    }
    if (value == 0)
    {
        if (put_corner(board,player))
        {
            coordinate = get_corner_coordinate(board,player);
        }
        else
        {
            while (board.get_cell_color(coordinate.first,coordinate.second)==enemy_color || cell_value[coordinate.first][coordinate.second] < value)
            {
                while (1)
                {
                    coordinate.first = mt_rand()%5;
                    coordinate.second = mt_rand()%6;
                    if (!((coordinate.first==0 && coordinate.second==0)||(coordinate.first==0 && coordinate.second==5)||(coordinate.first==4 && coordinate.second==0)||(coordinate.first==4 && coordinate.second==5))) break;
                }
            }
        }
    }
    index[0] = coordinate.first;
    index[1] = coordinate.second;
}

bool put_corner(Board board, Player player)
{
    if (board.get_cell_color(0,0)=='w' || board.get_cell_color(0,5)=='w' || board.get_cell_color(4,0)=='w' || board.get_cell_color(4,5)=='w')
    {
        return 1;
    }
    return 0;
}

bool put_edge(Board board,Player player)
{

    for (int i=0;i<5;i++)
    {
        if (board.get_cell_color(i,0) == 'w' || board.get_cell_color(i,5) == 'w') return 1;
    }
    for (int j=0;j<6;j++)
    {
        if (board.get_cell_color(0,j) == 'w' || board.get_cell_color(4,j) == 'w') return 1;
    }
    return 0;
}

Position get_edge_coordinate(Board board,Player player)
{
    mt19937 mt_rand(time(NULL));
    int row=-1,col=-1;
    while (board.get_cell_color(row,col)!='w')
    {
        row=1;col=1;
        while ((row!=0 && row!=4) && (col!=0 && col!=5))
        {
            row = mt_rand()%5;
            col = mt_rand()%6;
        }
    }
    Position p(row,col);
    return p;

}
Position get_corner_coordinate(Board board,Player player)
{
    if (board.get_cell_color(0,0) == 'w') {Position p(0,0); return p;}
    else if (board.get_cell_color(4,0) == 'w') {Position p(4,0); return p;}
    else if (board.get_cell_color(0,5) == 'w') {Position p(0,5); return p;}
    else if (board.get_cell_color(4,5) == 'w') {Position p(4,5); return p;}

}


void update_cell_value(Board board,Player player,int (&cell_value)[5][6])
{
    char enemy_color=(player.get_color()=='r')?'b':'r';
    for (int i=0;i<5;i++)
    {
        for (int j=0;j<6;j++)
        {
            if (board.get_cell_color(i,j)==enemy_color) continue;
            else if (board.get_cell_color(i,j) == 'w')
            {
                if (i-1>=0)
                {
                    if (board.get_orbs_num(i-1,j) == board.get_capacity(i-1,j)-1 && board.get_cell_color(i-1,j)==enemy_color)
                    {
                        cell_value[i][j]==-10;
                        continue;
                    }
                }
                if (i+1<5)
                {
                    if (board.get_orbs_num(i+1,j) == board.get_capacity(i+1,j)-1 && board.get_cell_color(i+1,j)==enemy_color)
                    {
                        cell_value[i][j]==-10;
                        continue;
                    }
                }
                if (j-1>=0)
                {
                    if (board.get_orbs_num(i,j-1) == board.get_capacity(i,j-1)-1 && board.get_cell_color(i,j-1)==enemy_color)
                    {
                        cell_value[i][j]==-10;
                        continue;
                    }
                }
                if (j+1<6)
                {
                    if (board.get_orbs_num(i,j+1) == board.get_capacity(i,j+1)-1 && board.get_cell_color(i,j+1)==enemy_color)
                    {
                        cell_value[i][j]==-10;
                        continue;
                    }
                }
            }
            switch(board.get_capacity(i,j))
            {
                case(2):
                {
                    if (board.get_orbs_num(i,j) == board.get_capacity(i,j)-1 && board.get_cell_color(i,j) == player.get_color())
                    {
                        cell_value[i][j] = -3;
                    }
                    int critical_cell_num = 0;
                    if (i==0 && j==0)
                    {
                        if (board.get_orbs_num(1,0)==2 && board.get_cell_color(1,0)==enemy_color)
                            critical_cell_num+=1;
                        if (board.get_orbs_num(0,1)==2 && board.get_cell_color(0,1)==enemy_color)
                            critical_cell_num+=1;
                    }
                    else if (i==0 && j==5)
                    {
                        if (board.get_orbs_num(1,5)==2 && board.get_cell_color(1,5)==enemy_color)
                            critical_cell_num+=1;
                        if (board.get_orbs_num(0,4)==2 && board.get_cell_color(0,4)==enemy_color)
                            critical_cell_num+=1;
                    }
                    else if (i==4 && j==0)
                    {
                        if (board.get_orbs_num(3,0)==2 && board.get_cell_color(3,0)==enemy_color)
                            critical_cell_num+=1;
                        if (board.get_orbs_num(4,1)==2 && board.get_cell_color(4,1)==enemy_color)
                            critical_cell_num+=1;
                    }
                    else if (i==4 && j==5)
                    {
                        if (board.get_orbs_num(3,5)==2 && board.get_cell_color(3,5)==enemy_color)
                            critical_cell_num+=1;
                        if (board.get_orbs_num(4,4)==2 && board.get_cell_color(4,4)==enemy_color)
                            critical_cell_num+=1;
                    }
                    cell_value[i][j] = (critical_cell_num!=0)?cell_value[i][j]*(-critical_cell_num):cell_value[i][j];
                    break;
                }
                case(3):
                {
                    if (board.get_orbs_num(i,j) == board.get_capacity(i,j)-1 && board.get_cell_color(i,j) == player.get_color())
                    {
                        cell_value[i][j] = -2;
                    }
                    int critical_cell_num = 0;
                    if (i==0)
                    {
                        if (j==1)
                        {
                            if (board.get_orbs_num(0,j-1)==1 && board.get_cell_color(0,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(0,j+1)==2 && board.get_cell_color(0,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(1,j)==3 && board.get_cell_color(1,j)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else if (j==4)
                        {
                            if (board.get_orbs_num(0,j-1)==2 && board.get_cell_color(0,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(0,j+1)==1 && board.get_cell_color(0,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(1,j)==3 && board.get_cell_color(1,j)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else
                        {
                            if (board.get_orbs_num(0,j-1)==2 && board.get_cell_color(0,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(0,j+1)==2 && board.get_cell_color(0,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(1,j)==3 && board.get_cell_color(1,j)==enemy_color)
                                critical_cell_num+=1;
                        }

                    }
                    else if (j==0)
                    {
                        if (i==1)
                        {
                            if (board.get_orbs_num(i-1,0)==1 && board.get_cell_color(i-1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,0)==2 && board.get_cell_color(i+1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,1)==3 && board.get_cell_color(i,1)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else if (i==3)
                        {
                            if (board.get_orbs_num(i-1,0)==2 && board.get_cell_color(i-1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,0)==1 && board.get_cell_color(i+1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,1)==3 && board.get_cell_color(i,1)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else
                        {
                            if (board.get_orbs_num(i-1,0)==2 && board.get_cell_color(i-1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,0)==2 && board.get_cell_color(i+1,0)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,1)==3 && board.get_cell_color(i,1)==enemy_color)
                                critical_cell_num+=1;
                        }

                    }
                    else if (i==4)
                    {
                        if (j==1)
                        {
                            if (board.get_orbs_num(4,j-1)==1 && board.get_cell_color(4,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(4,j+1)==2 && board.get_cell_color(4,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(3,j)==3 && board.get_cell_color(3,j)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else if (j==4)
                        {
                            if (board.get_orbs_num(4,j-1)==2 && board.get_cell_color(4,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(4,j+1)==1 && board.get_cell_color(4,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(3,j)==3 && board.get_cell_color(3,j)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else
                        {
                            if (board.get_orbs_num(4,j-1)==2 && board.get_cell_color(4,j-1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(4,j+1)==2 && board.get_cell_color(4,j+1)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(3,j)==3 && board.get_cell_color(3,j)==enemy_color)
                                critical_cell_num+=1;
                        }

                    }
                    else if (j==5)
                    {
                        if (i==1)
                        {
                            if (board.get_orbs_num(i-1,5)==1 && board.get_cell_color(i-1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,5)==2 && board.get_cell_color(i+1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,4)==3 && board.get_cell_color(i,4)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else if (i==3)
                        {
                            if (board.get_orbs_num(i-1,5)==2 && board.get_cell_color(i-1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,5)==1 && board.get_cell_color(i+1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,4)==3 && board.get_cell_color(i,4)==enemy_color)
                                critical_cell_num+=1;
                        }
                        else
                        {
                            if (board.get_orbs_num(i-1,5)==1 && board.get_cell_color(i-1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i+1,5)==2 && board.get_cell_color(i+1,5)==enemy_color)
                                critical_cell_num+=1;
                            if (board.get_orbs_num(i,4)==3 && board.get_cell_color(i,4)==enemy_color)
                                critical_cell_num+=1;
                        }

                    }
                    cell_value[i][j] = (critical_cell_num!=0)?cell_value[i][j]*(-critical_cell_num):cell_value[i][j];
                    break;
                }
                case(4):
                {
                    if (board.get_orbs_num(i,j) == board.get_capacity(i,j)-1 && board.get_cell_color(i,j) == player.get_color())
                    {
                        cell_value[i][j] = -2;
                    }
                    int critical_cell_num = 0;
                    if (board.get_orbs_num(i-1,j)==3 && board.get_cell_color(i-1,j)==enemy_color)
                        critical_cell_num+=1;
                    if (board.get_orbs_num(i+1,j)==3 && board.get_cell_color(i+1,j)==enemy_color)
                        critical_cell_num+=1;
                    if (board.get_orbs_num(i,j-1)==3 && board.get_cell_color(i,j-1)==enemy_color)
                        critical_cell_num+=1;
                    if (board.get_orbs_num(i,j+1)==3 && board.get_cell_color(i,j+1)==enemy_color)
                        critical_cell_num+=1;
                    cell_value[i][j] = (critical_cell_num!=0)?cell_value[i][j]*(-critical_cell_num):cell_value[i][j];
                    break;
                }
            }
        }
    }
}
