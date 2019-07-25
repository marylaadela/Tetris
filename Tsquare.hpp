#ifndef TSQUARE_HPP_
#define TSQUARE_HPP_

#include <iostream>
#include <vector>

class Tsquare{
        friend bool can_tetrimino_move(int **board, int *wsk);
        friend void add_tetrimino_to_board (int **board, std::vector<Tsquare> new_element, std::vector<int> &recording);
        public:
        Tsquare(int y = 0, int x = 0, int colour = 0);
        private:
        int x;
        int y;
        int x1;
        int colour;
};

#endif                                                                              
