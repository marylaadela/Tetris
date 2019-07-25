SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o

CXX = g++
CXXFLAGS = -std=c++11  
OBJS = main.o

__start__: Tetris
	@./Tetris

Tetris: $(OBJS)
	@$(CXX) $(OBJS) -o Tetris -lncurses

$(OBJS): main.cpp draw.hpp Tsquare.hpp
	@$(CXX) $(CXXFLAGS) -c main.cpp -lncurses  

clean:
	@rm -f *.o Tetris

.PHONY: all clean
