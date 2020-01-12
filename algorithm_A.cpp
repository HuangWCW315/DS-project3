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
int evaluate(int row, int col, Board board, Player player);
int game_result(Board board, Player player);
int check_adjency_critical(int row, int col, Board board, char player);
int block_type(int row, int col);

int Find(int row, int col, Board board, Player player, Player enemy,int depth, int T);
int find(int row, int col, Board board, Player player, Player enemy, bool choose, int depth,int T, int alpha, int beta);
    // T should be odd

void algorithm_A(Board board, Player player, int index[])
{
    srand(time(NULL));
    int row, col;
    char color = player.get_color();
    char enemy_color;
    int score = ILLEGAL;

    if (color == RED)   enemy_color = BLUE;
    else enemy_color = RED;

    Player enemy(enemy_color);

/*    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = Find(i, j, board, player, enemy, 0, 3)) && new_score != GAME_BROKEN)
            {

                    index[0] = i;
                    index[1] = j;
                    score = new_score;
            }
   */         
            
 //           cout << "A  (" << i << ',' << j << ") : " << new_score << endl;      
   //     }
  //      cout << "(row, col) = (" << index[0] << ',' << index[1] << ")\n";

  //          cout << "A : "<< score << endl;
  //          cout << "(row, col) = (" << index[0] << ',' << index[1] << ")\n";
 //           system("pause");  
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = evaluate(i, j, board, player)))
            {

                index[0] = i;
                index[1] = j;
                score = new_score;

            }
        }

//    system("pause");

}


void algorithm_F(Board board, Player player, int index[]){
    
/*
    srand(time(NULL));
    int row, col;
    int color = player.get_color();

    int score = ILLEGAL;

 
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = evaluate(i, j, board, player)))
            {

                index[0] = i;
                index[1] = j;
                score = new_score;

            }
        }*/
    cout << "F\n";
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
            if (score < (new_score = find(i, j, board, player, enemy, ENEMY, 0, 1, alpha, beta)) && new_score != GAME_BROKEN)
            {

                    index[0] = i;
                    index[1] = j;
                    score = new_score;
            }
            if (alpha < new_score)  alpha = new_score;
            if (beta <= alpha)  return;
            
            
  //          cout << "F  (" << i << ',' << j << ") : " << new_score << endl;      
        }
 //       cout << "(row, col) = (" << index[0] << ',' << index[1] << ")\n";
 //       cout << "F:" << score << endl;
  //          system("pause");  
//    system("pause");

}

int evaluate(int row, int col, Board board, Player player)
{
    int score = 0;
    char player_color = player.get_color();

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

int Find(int row, int col, Board board, Player player, Player enemy, int depth, int T)
{
    char player_color;

    if (depth % 2 == 0) 
    {
        
        player_color = player.get_color();
        if (board.get_cell_color(row, col) != player_color && board.get_cell_color(row, col) != 'w')
            return ILLEGAL;
        else board.place_orb(row, col, &player);
    }

    int score = ILLEGAL;

        
    if (depth >= T - 1)     // achieve recurrsive depth
    {
   //     cout << "A\n";
        for (int i = 0; i < ROW; i++)        // highest score
            for (int j = 0; j < COL; j++)
            {
                int new_score;
                if (score < (new_score = evaluate(i, j, board, player)))
                    score = new_score;         
            }
        return score;
    }

               
    if (depth % 2 == 0)     // enemy turn
    {
        int position[2] = {0, 0};
        for (int i = 0; i < ROW; i++)       // find least score
            for (int j = 0; j < COL; j++)
            {
                int new_score;
     /*           if (counter >= 55 && T == 5 && i == 3 && j == 5)
                {

                    board.print_current_board(3, 5, 0);
                    system("pause");
                }*/
   /*             if (counter >= 55 && T == 5 && i == 4 && j == 0)
                {
                    Board board_copy = board;
                    board.print_current_board(4, 0, 0);
                    cout << board.get_cell_color(4,0) << endl;
                     cout << enemy.get_color() << endl;
                     board_copy.place_orb(4, 0, &enemy);
                     cout << board.get_cell_color(4,0) << endl;
                     cout << enemy.get_color() << endl;
                     board_copy.print_current_board(4, 0, 1);
             //       system("pause");
                }*/
                if (score < (new_score = evaluate(i, j, board, enemy)) && new_score != ILLEGAL)
                {
                    score = new_score;
                    position[0] = i;
                    position[1] = j;
                }
                
            }
         
        board.place_orb(position[0], position[1], &enemy);


        if (game_result(board, player) == LOSE)   return LOSE_GAME;
        depth++;
    }
    score = ILLEGAL;
    // my turn
    
    for (int i = 0; i < ROW; i++)        // highest score
        for (int j = 0; j < COL; j++)
        {
            int new_score;
            if (score < (new_score = Find(i, j, board, player, enemy, depth + 1, T)))
                score = new_score;         
        }
    return score;
    
}

int find(int row, int col, Board board, Player player, Player enemy, bool choose, int depth, int T, int alpha, int beta)
{
    int score = ILLEGAL;
    if (depth == T)
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
    if (choose == MY)
    {
        if (place_legal(row, col, board, enemy))   board.place_orb(row, col, &enemy);
        else return GAME_BROKEN;

        if (game_result(board, player) == WIN)  return WIN_GAME;
        else if (game_result(board, player) == LOSE)  return LOSE_GAME;

        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                int new_score = find(i, j, board, player, enemy, !choose, depth + 1, T, alpha, beta);
                if (score < new_score && new_score != GAME_BROKEN)
                {
                    score = new_score;
                }
                if (alpha < new_score)  alpha = new_score;
                if (beta <= alpha)  return score;
            }
        return score;
    
    }
    if (choose == ENEMY)
    {
        if (place_legal(row, col, board, player))   board.place_orb(row, col, &player);
        else return GAME_BROKEN;
         if (game_result(board, player) == WIN)  return WIN_GAME;
        else if (game_result(board, player) == LOSE)  return LOSE_GAME;
        score = ENEMY_WIN_GAME;
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                int new_score = find(i, j, board, player, enemy, !choose, depth + 1, T, alpha, beta);
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