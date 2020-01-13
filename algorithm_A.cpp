#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

#define ENEMY_WIN_GAME 200000000
#define WIN_GAME 100000000
#define LOSE_GAME -100000000
#define ILLEGAL -200000000
#define GAME_BROKEN -300000000
#define WIN 1
#define LOSE 2
#define NOT_END 3
#define CORNER 1
#define EDGE 2
#define INT_INFINITY 1000000000
#define MY true
#define ENEMY false


int evaluate(int row, int col, Board board, Player player);
int game_result(Board board, Player player);
int check_adjency_critical(int row, int col, Board board, char player);
int block_type(int row, int col);

int check_adjency_critical_connected(bool record_counted[][COL], int row, int col, Board board, char player);

int find(int row, int col, Board board, Player player, Player enemy, bool turn, int depth,int MAX_depth, int alpha, int beta);
    // MAX_depth should be odd

void algorithm_A(Board board, Player player, int index[])
{
    srand(time(NULL));
    int row, col;
    char color = player.get_color();
    char enemy_color;
    int score = ILLEGAL;
    int alpha = -(INT_INFINITY);
    int beta = INT_INFINITY;
    if (color == RED)   enemy_color = BLUE;
    else enemy_color = RED;
    Player enemy(enemy_color);

    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = find(i, j, board, player, enemy, ENEMY, 0, 3, alpha, beta)) && new_score != GAME_BROKEN)
            {

                    index[0] = i;
                    index[1] = j;
                    score = new_score;
            }
            if (alpha < new_score)  alpha = new_score;
            if (beta <= alpha)  return;    
        }
}

int evaluate(int row, int col, Board board, Player player)
{
    int score = 0;
    char player_color = player.get_color();
    bool record_counted[ROW][COL] = {false};

    if (board.get_cell_color(row, col) != player_color && board.get_cell_color(row, col) != 'w')
        return ILLEGAL;
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
                if (board.get_capacity(i, j) - 1 == board.get_orbs_num(i, j))
                {
                    score += check_adjency_critical_connected(record_counted, i, j, board, player_color);
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
                score -= 5 - board.get_capacity(row, col + 1);  // right

    return score;
}


int check_adjency_critical_connected(bool record_counted[][COL], int row, int col, Board board, char player)
{
    int number = 0;
    if (row < ROW - 1)    // down
        if (board.get_cell_color(row + 1, col) == player)
            if (board.get_orbs_num(row + 1, col) == board.get_capacity(row + 1, col) - 1 && !record_counted[row + 1][col])
            {
                number++;
                record_counted[row + 1][col] = true;
            }
                
                            
    if (row > 0)    
        if (board.get_cell_color(row - 1, col) == player)
            if (board.get_orbs_num(row - 1, col) == board.get_capacity(row - 1, col) - 1 && !record_counted[row - 1][col])
            {
                number++;
                record_counted[row - 1][col] = true;
            }
                            // up
    if (col > 0)    
        if (board.get_cell_color(row, col - 1) == player)
            if (board.get_orbs_num(row, col - 1) == board.get_capacity(row, col - 1) - 1 && !record_counted[row][col - 1])
            {
                number++;
                record_counted[row][col - 1] = true;
            }
                            // left
    if (col < COL - 1) 
        if (board.get_cell_color(row, col + 1) == player)
            if (board.get_orbs_num(row, col + 1) == board.get_capacity(row, col + 1) - 1 && !record_counted[row][col + 1])
            {
                number++;
                record_counted[row][col + 1] = true;
            }     // right

    return 2 * number;
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

bool place_legal(int row, int col, Board board, Player player)
{
    if (board.get_cell_color(row, col) != player.get_color() && board.get_cell_color(row, col) != 'w')
        return false;
    else return true;
}


int find(int row, int col, Board board, Player player, Player enemy, bool turn, int depth, int MAX_depth, int alpha, int beta)
{
    int score = ILLEGAL;
    if (depth == MAX_depth)
    {
        if (place_legal(row, col, board, enemy))   board.place_orb(row, col, &enemy);
        else return GAME_BROKEN;
        if (game_result(board, player) == WIN)  return WIN_GAME;
        else if (game_result(board, player) == LOSE)  return LOSE_GAME;
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                int new_score;
                if (score < (new_score = evaluate(i, j, board, player)))
                {
                    score = new_score;
                }
            }
        return score;
    }
    if (turn == MY)
    {
        if (place_legal(row, col, board, enemy))   board.place_orb(row, col, &enemy);
        else return GAME_BROKEN;

        if (game_result(board, player) == WIN)  return WIN_GAME;
        else if (game_result(board, player) == LOSE)  return LOSE_GAME;

        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                int new_score = find(i, j, board, player, enemy, !turn, depth + 1, MAX_depth, alpha, beta);
                if (score < new_score && new_score != GAME_BROKEN)
                {
                    score = new_score;
                }
                if (alpha < new_score)  alpha = new_score;
                if (beta <= alpha)  return score;
            }
        return score;
    
    }
    if (turn == ENEMY)
    {
        if (place_legal(row, col, board, player))   board.place_orb(row, col, &player);
        else return GAME_BROKEN;
         if (game_result(board, player) == WIN)  return WIN_GAME;
        else if (game_result(board, player) == LOSE)  return LOSE_GAME;
        score = ENEMY_WIN_GAME;
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                int new_score = find(i, j, board, player, enemy, !turn, depth + 1, MAX_depth, alpha, beta);
                if (score > new_score && new_score >= ILLEGAL)
                {
                    score = new_score;
                }
                if (beta > new_score && new_score > ILLEGAL)  beta = new_score;
                if (beta <= alpha)  return score;
            }
        return score;
    
    }
    return score;
}