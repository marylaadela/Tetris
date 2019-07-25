#ifndef DRAW_HPP_
#define DRAW_HPP_

using VECTOR = std::vector<Tsquare>;

VECTOR draw_smalls (int *wsk);
VECTOR draw_bigs (int *wsk);
VECTOR draw_tower (int *wsk);
VECTOR draw_j (int *wsk);
VECTOR draw_l (int *wsk);
VECTOR draw_s (int *wsk);
VECTOR draw_z (int *wsk);
VECTOR draw_t (int *wsk);
VECTOR which_tetrimino(int *wsk);
void run_time(int **board, int *wsk, int *score);
void give_sign(char c, int **board, int *wsk);
VECTOR which_tetrimino(int *wsk);
void draw_stats (int *score);
void make_windows ();
void setup_screen();
VECTOR fall_down_tetrimino(int **board, int *wsk, int *score);
int delete_full_lines(int **board, std::vector<int> &recording);
void clean_up (int **board, int lines_to_remove);
void show_tetrimino(int **board);
bool check_end_game(int **board);
void end_tetrimino();

#endif               
