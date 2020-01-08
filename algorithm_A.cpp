#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

#define WIN_GAME 100000000
#define LOSE_GAME -100000000
#define WIN 1
#define LOSE 2
#define NOT_END 3
#define CORNER 1
#define EDGE 2
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
int evaluate(int row, int col, Board board, Player player, int depth);
int game_result(Board board, Player player);
int check_adjency_critical(int row, int col, Board board, char player);
int block_type(int row, int col);

void algorithm_A(Board board, Player player, int index[]){

    // cout << board.get_capacity(0, 0) << endl;
    // cout << board.get_orbs_num(0, 0) << endl;
    // cout << board.get_cell_color(0, 0) << endl;
    // board.print_current_board(0, 0, 0);

    //////////// Random Algorithm ////////////
    // Here is the random algorithm for your reference, you can delete or comment it.
    srand(time(NULL));
    int row, col;
    int color = player.get_color();
    
 /*   cin >> row >> col;
    cout << evaluate(row, col, board, player, 0) << endl;*/
   /* while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }*/

    int score = LOSE_GAME;

    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = evaluate(i, j, board, player, 0)))
            {

                index[0] = i;
                index[1] = j;
                score = new_score;

            }
        }

//    system("pause");

}

int evaluate(int row, int col, Board board, Player player, int depth)
{
    int score = 0;
    char player_color = player.get_color();

    if (board.get_cell_color(row, col) != player_color && board.get_cell_color(row, col) != 'w')
        return LOSE_GAME;
    board.place_orb(row, col, &player);
    int result = game_result(board, player);
    if (result == WIN) score += WIN_GAME;
    else if (result == LOSE) score += LOSE_GAME;

    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int temp;
            if (player_color == board.get_cell_color(i,j))
            {
                score += board.get_orbs_num(i, j);
                temp = check_adjency_critical(i, j, board, player_color);
                score += temp;
                if (temp == 0)
                {
                    if (block_type(i, j) == CORNER) score += 3;
                    else if (block_type(i, j) == EDGE) score += 2;
                    
                    if (board.get_capacity(i, j) - 1 == board.get_orbs_num(i, j))
                        score += 2;
                }
                    
            }
                
        }

    return score;
}

int game_result(Board board, Player player)
{
    char player_color = player.get_color();
    int me = 0;
    int enemy = 0;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(board.get_cell_color(i,j) == player_color) me++;
            else if (board.get_cell_color(i,j) != 'w') enemy++;
        }
    }
    if (enemy == 0 && me != 0) return WIN;
    if (enemy != 0 && me == 0) return LOSE;
    return NOT_END; 
}

int check_adjency_critical(int row, int col, Board board, char player)
{
    int score = 0;
    if (row < ROW - 1)   
        if (board.get_cell_color(row + 1, col) != player && board.get_cell_color(row + 1, col) != 'w')
            if (board.get_orbs_num(row + 1, col) == board.get_capacity(row + 1, col) - 1)
                score -= 5 - board.get_capacity(row + 1, col);
                            // down
    if (row > 0)    
        if (board.get_cell_color(row - 1, col) != player && board.get_cell_color(row - 1, col) != 'w')
            if (board.get_orbs_num(row - 1, col) == board.get_capacity(row - 1, col) - 1)
                score -= 5 - board.get_capacity(row - 1, col);
                            // up
    if (col > 0)    
        if (board.get_cell_color(row, col - 1) != player && board.get_cell_color(row, col - 1) != 'w')
            if (board.get_orbs_num(row, col - 1) == board.get_capacity(row, col - 1) - 1)
                score -= 5 - board.get_capacity(row, col - 1);
                            // left
    if (col < COL - 1) 
        if (board.get_cell_color(row, col + 1) != player && board.get_cell_color(row, col + 1) != 'w')
            if (board.get_orbs_num(row, col + 1) == board.get_capacity(row, col + 1) - 1)
                score -= 5 - board.get_capacity(row, col + 1);// right

    return score;
}

int block_type(int row, int col)
{
    if (row == 0 || col == 0 || row == ROW - 1 || col == COL - 1)
    {
        if (row == 0 && col == 0 || row == 0 && col == COL - 1 || 
        row == ROW - 1 && col == 0 || row == ROW - 1 && col == COL - 1)     // corner
            return CORNER;
        else return EDGE;            // edge
    }
    return 0;
}