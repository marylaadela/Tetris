#include <ncurses.h>
#include <unistd.h>
#include "Tsquare.hpp"
#include "draw.hpp"
using namespace std;

static WINDOW *Tetris;
static WINDOW *Score_status;
static int seconds;

Tsquare::Tsquare (int line, int column, int paint):     y{line},
                                                        x{column},
                                                        x1{column+1},
                                                        colour{paint}
{       mvwprintw(Tetris,y,x,"11");
        curs_set(0);    }


static int x, y, remember;

int main(){

	int score = 0;

	int remember_turning {0};

	setup_screen();
	make_windows();
	draw_stats(&score);

	int **board = new int *[40];
	for (int i = 0; i < 40; i++)
		board[i] = new int[40];

	for (int i = 0; i < 40; i++)
		for (int j = 0; j < 40; j++)
			board[i][j] = 0;
	VECTOR new_on_board; 
	vector<int> recording(40);
	srand(time(0));

	bool time_to_end = true;
	while(time_to_end){

	        remember = rand() % 8;
		remember_turning = 0;
		x = 20;
		y = 3;
		time_to_end = check_end_game(board);
		new_on_board = fall_down_tetrimino(board, &remember_turning, &score);
		add_tetrimino_to_board (board, new_on_board, recording);
		score = score + 10*delete_full_lines(board, recording);
      		draw_stats(&score);
	}



	for (int i = 0; i < 40; i++)
		delete []board[i];
	delete []board;


	end_tetrimino();
}

bool check_end_game(int **board){
	bool check = true;
	if (board[y][x]){
		mvwprintw(Tetris,y,x,"GAME OVER");
		wrefresh(Tetris);
		sleep(1);
		end_tetrimino();
		check = false;
 	}
	return check;
}
int delete_full_lines (int **board, vector<int> &recording){
	int full_line = 0;
	for (int i = 40; i > 0;i--){
		if (recording[i] == 18){
		for(int m = 2; m < 39; m++)
			board[i][m] = 2;
		show_tetrimino(board);
		sleep(1);
		full_line++;
		for (int j = i; j > 0; j--)
			for (int k = 0; k <= 40; k++)
				board[j][k] = board[j-1][k];
		for (int l = i; l > 0; l--)
                        recording[l] = recording[l-1];
		i++;
	}
	}
	wclear(Tetris);
 	show_tetrimino(board);
	
	return full_line;
}
void show_tetrimino(int **board){
	for (int i = 0; i <= 38; i++)
		for (int j = 0; j <= 38; j++){

		if (board[i][j] != 0){
			wattron(Tetris, COLOR_PAIR(board[i][j]));
			mvwprintw(Tetris,i,j,"11");		
			wattroff(Tetris, COLOR_PAIR(board[i][j]));  
		}
	j++;}

	wborder(Tetris, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
                ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

        wrefresh(Tetris);
        wrefresh(Score_status);

}
void draw_stats (int *score){

	wborder(Score_status, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
                ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

	wattron(Score_status,COLOR_PAIR(8));
	mvwprintw(Score_status, 1, 1, "Score");

	wattron(Score_status,COLOR_PAIR(9));
	mvwprintw(Score_status, 1, 7, "%d", *score);
	mvwprintw(Score_status, 2, 1, "Q");

	wattron(Score_status,COLOR_PAIR(8));
	mvwprintw(Score_status, 2, 2, "uit");
	wrefresh(Score_status);
}
void setup_screen (){
        initscr();
        noecho();
        cbreak();
        raw();
        nodelay (Tetris, TRUE);
        nodelay(Score_status, TRUE);
	notimeout(Tetris, TRUE);
        keypad(stdscr, true);



	if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
 	}	
	start_color();

	init_pair(1, COLOR_BLUE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(4, COLOR_RED, COLOR_RED);
        init_pair(5, COLOR_CYAN, COLOR_CYAN);
        init_pair(6, COLOR_GREEN, COLOR_GREEN);
        init_pair(7, COLOR_YELLOW, COLOR_YELLOW);

        init_pair(8, COLOR_RED, COLOR_BLACK);
        init_pair(9, COLOR_YELLOW, COLOR_BLACK);

}
void end_tetrimino(){
	delwin(Tetris);
	Tetris = NULL;
	delwin(Score_status);
	Score_status= NULL;
	endwin();
}
void make_windows (){

	if (Tetris){
		delwin(Tetris);
		Tetris = NULL;
	}
	if (Score_status){
                delwin(Score_status);
                Score_status = NULL;
        }
	wclear(Tetris);
	wrefresh(Tetris);
	

	if (!initscr()){
		cout << "initsr(): error";
		exit(1);
	}
	if (LINES < 44 || COLS < 44){
		cout << "terminal too small!!";
		endwin();
		exit(1);
	}	

	Tetris = newwin (40, 40 , 2, 2);
	Score_status = newwin (5,15, 30, 45);

	wrefresh(Tetris);
	wrefresh(Score_status);
}


bool can_tetrimino_move (int **tab, int *wsk){
	VECTOR to_check;
	to_check = which_tetrimino(wsk);
	for (auto tested: to_check){
		if(tested.x == 0 || tested.x == 38)
			return false;
		if(tested.y ==39)
			return false;
		if (tab[tested.y][tested.x] != 0)
			return false;
	}
	
	return true;
}


void add_tetrimino_to_board (int **board, VECTOR new_element, vector<int> &recording){
	for (auto component: new_element){
		board[component.y][component.x] = component.colour;
		recording[component.y] = recording[component.y] + 1;}}


VECTOR fall_down_tetrimino(int **board, int *wsk, int *score){

	bool check;
	VECTOR tetrimino;

	for (y; y < 39; ++y){
		check = can_tetrimino_move(board, wsk);
		if (check == false) 
			break;
		wclear(Tetris);
		show_tetrimino(board);
		which_tetrimino(wsk);
		wrefresh(Tetris);
		run_time(board, wsk, score);

	}
	y--;
	tetrimino = which_tetrimino(wsk);	
	wrefresh(Tetris);
	show_tetrimino(board);


	return tetrimino;
}


void run_time(int **board, int *wsk, int *score){
	int milisecond = 1000;

	if (*score < 50)
		seconds = 180;
	else if (*score % 50 == 0)
		seconds = seconds - 20;	
		
	milisecond = milisecond * seconds;
	nodelay(Tetris, TRUE);
	clock_t start = clock();
	while (clock() < start + milisecond){
		char c;
		if (c = wgetch(Tetris))
			if (c != 's')
				give_sign(c, board, wsk);
			else
				break;
	}
}


void give_sign(char c,int **board, int *wsk){
	bool check;

	if (c == 'a'){
		x = x - 2;
		check = can_tetrimino_move(board, wsk);
		if (check == false)
			x = x + 2;
		wclear(Tetris);
		show_tetrimino(board);

		which_tetrimino(wsk);
		wrefresh(Tetris);
	}
	else if (c == 'd'){
		x = x + 2;
		check = can_tetrimino_move(board,wsk);
                if (check == false)
                        x = x - 2;
                wclear(Tetris);
		show_tetrimino(board);

		which_tetrimino(wsk);
		wrefresh(Tetris);
	}
	else if(c == 'm'){	
		if (*wsk <= 3)
			*wsk = *wsk + 1;
		check = can_tetrimino_move(board,wsk);
                if (check == false)
			*wsk = *wsk - 1;
		else if (*wsk > 3)
			*wsk = 0;

		wclear(Tetris);
                show_tetrimino(board);

                which_tetrimino(wsk);
                wrefresh(Tetris);

	}
	else if(c == 'q' || c == 'Q'){
		endwin(); 
		delwin(Tetris); 
		delwin(Score_status); 
		clear();
		exit(1);
	}

}


VECTOR which_tetrimino(int *wsk){
	VECTOR new_tetrimino;
	
	switch (remember){
	case 0:
		new_tetrimino = draw_smalls(wsk);
		break;
	case 1:
		new_tetrimino = draw_bigs(wsk);
		break;
	case 2:
		new_tetrimino = draw_tower(wsk);
		break;
	case 3:
		new_tetrimino = draw_j(wsk);
		break;
	case 4:
		new_tetrimino = draw_l(wsk);
		break;
	case 5:
		new_tetrimino = draw_t(wsk);
		break;
	case 6:
		new_tetrimino = draw_s(wsk);
		break;
	default:
		new_tetrimino = draw_z(wsk);
		break;
	}
	return new_tetrimino;
}


VECTOR draw_smalls (int *wsk){
	
	wattron(Tetris, COLOR_PAIR (1));
	
	VECTOR tetrimino = { Tsquare(y,x,1) };
	
	wattroff(Tetris, COLOR_PAIR (1));

	return tetrimino;
}


VECTOR draw_bigs (int *wsk){
	
        wattron(Tetris, COLOR_PAIR (2));
        
        VECTOR tetrimino = { 
					Tsquare(y,x,2),
					Tsquare(y,x+2,2),
					Tsquare(y+1, x,2),
					Tsquare(y+1, x+2,2)	};
        
	wattroff(Tetris, COLOR_PAIR (2));

	return tetrimino;
}


VECTOR draw_tower (int *wsk){
	
	wattron(Tetris, COLOR_PAIR (3));
	
	if (*wsk % 2 == 0){       
	VECTOR tetrimino = {
                                        Tsquare(y+1,x-2,3),
                                        Tsquare(y+1,x,3),
                                        Tsquare(y+1, x+2, 3),
                                        Tsquare(y+1, x+4, 3)       }; 
 	wattroff(Tetris, COLOR_PAIR (3));
        return tetrimino;
	}else {
	VECTOR tetrimino = { 
                                        Tsquare(y,x,3),
                                        Tsquare(y+1,x,3),
                                        Tsquare(y+2, x, 3),
                                        Tsquare(y+3, x, 3)       };
	
	wattroff(Tetris, COLOR_PAIR (3));

        return tetrimino;
	}
}


VECTOR draw_j (int *wsk){

	wattron(Tetris, COLOR_PAIR (4));
        
	if (*wsk % 4 == 0){
	VECTOR tetrimino = { 
                                        Tsquare(y,x,4),
                                        Tsquare(y+1,x,4),
                                        Tsquare(y+2, x,4),
                                        Tsquare(y+2, x-2,4)       };
	wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;
	}else if (*wsk % 4 == 1){
	VECTOR tetrimino = {
                                        Tsquare(y+1,x-4,4),
                                        Tsquare(y+2,x-4,4),
                                        Tsquare(y+2, x-2, 4),
                                        Tsquare(y+2, x, 4)       };
	wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;
	}else if (*wsk % 4 == 2){
	VECTOR tetrimino = {
                                        Tsquare(y,x,4),
                                        Tsquare(y,x-2,4),
                                        Tsquare(y+1, x-2, 4),
                                        Tsquare(y+2, x-2, 4)       };
	wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;
	}else {
	VECTOR tetrimino = {
                                        Tsquare(y+1,x,4),
                                        Tsquare(y+1,x-2,4),
                                        Tsquare(y+1, x-4, 4),
                                        Tsquare(y+2, x, 4)       };

        wattroff(Tetris, COLOR_PAIR (4));

	return tetrimino;
}
}

VECTOR draw_l (int *wsk){
  
	wattron(Tetris, COLOR_PAIR (5));

	if (*wsk % 4 == 0){
	VECTOR tetrimino = { 
                                        Tsquare(y,x,5),
                                        Tsquare(y+1,x,5),
                                        Tsquare(y+2, x,5),
                                        Tsquare(y+2, x+2,5)       };

	wattroff(Tetris, COLOR_PAIR (5));

        return tetrimino;

	}else if (*wsk % 4 == 1){
	VECTOR tetrimino = {
                                        Tsquare(y+1,x-2,5),
                                        Tsquare(y+1,x,5),
                                        Tsquare(y+1, x+2, 5),
                                        Tsquare(y+2, x-2, 5)       };
	wattroff(Tetris, COLOR_PAIR (5));

        return tetrimino;

        }else if (*wsk % 4 == 2){
	VECTOR tetrimino = {
                                        Tsquare(y,x,5),
                                        Tsquare(y,x+2,5),
                                        Tsquare(y+1, x+2, 5),
                                        Tsquare(y+2, x+2, 5)       };
  	wattroff(Tetris, COLOR_PAIR (5));

        return tetrimino;

	}else if (*wsk % 4 == 3){
	VECTOR tetrimino = {
                                        Tsquare(y+2,x-2,5),
                                        Tsquare(y+2,x,5),
                                        Tsquare(y+2, x+2, 5),
                                        Tsquare(y+1, x+2, 5)       };
	wattroff(Tetris, COLOR_PAIR (5));

	return tetrimino;
}}


VECTOR draw_s (int *wsk){

        wattron(Tetris, COLOR_PAIR (6));

	if (*wsk % 2 == 0){
 	VECTOR tetrimino = { 
                                        Tsquare(y,x,6),
                                        Tsquare(y,x+2,6),
                                        Tsquare(y+1, x,6),
                                        Tsquare(y+1, x-2,6)       };
       wattroff(Tetris, COLOR_PAIR (6));

        return tetrimino;
	}else if (*wsk % 2 == 1){
	VECTOR tetrimino = {
                                        Tsquare(y-1, x,6),
                                        Tsquare(y, x,6),
                                        Tsquare(y, x+2,6),
                                        Tsquare(y+1, x+2,6)       };

        wattroff(Tetris, COLOR_PAIR (6));

	return tetrimino;
}}


VECTOR draw_z (int *wsk){

	wattron(Tetris, COLOR_PAIR (7));
	
	if (*wsk % 2 == 0){
	VECTOR tetrimino = { 
                                        Tsquare(y,x,7),
                                        Tsquare(y,x+2,7),
                                        Tsquare(y+1, x+2,7),
                                        Tsquare(y+1, x+4,7)       };
	wattroff(Tetris, COLOR_PAIR (7));

        return tetrimino;

	}else if (*wsk % 2 == 1){
	VECTOR tetrimino = {
                                        Tsquare(y-1,x+4,7),
                                        Tsquare(y,x+2,7),
                                        Tsquare(y, x+4,7),
                                        Tsquare(y+1, x+2,7)       };


        wattroff(Tetris, COLOR_PAIR (7));

	return tetrimino;
}
}

VECTOR draw_t (int *wsk){

        wattron(Tetris, COLOR_PAIR (4));

	if (*wsk % 4 == 0){
	VECTOR tetrimino = { 
                                        Tsquare(y,x,4),
                                        Tsquare(y,x+2,4),
                                        Tsquare(y, x+4,4),
                                        Tsquare(y+1, x+2,4)       };
       wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;

      	}else if (*wsk % 4 == 1){
	VECTOR tetrimino = {
                                        Tsquare(y-1,x+4,4),
                                        Tsquare(y,x+2,4),
                                        Tsquare(y, x+4,4),
                                        Tsquare(y+1, x+4,4)       };
       wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;

	}else if (*wsk % 4 == 2){
	VECTOR tetrimino = {
                                        Tsquare(y,x+2,4),
                                        Tsquare(y+1,x,4),
                                        Tsquare(y+1, x+2,4),
                                        Tsquare(y+1, x+4,4)       };
  
       wattroff(Tetris, COLOR_PAIR (4));

        return tetrimino;
    	}else if (*wsk % 4 == 3){
	VECTOR tetrimino = {
                                        Tsquare(y-1,x+2,4),
                                        Tsquare(y,x+2,4),
                                        Tsquare(y, x+4,4),
                                        Tsquare(y+1, x+2,4)       };


        wattroff(Tetris, COLOR_PAIR (4));

	return tetrimino;
}}
