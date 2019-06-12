#include <ncurses.h>
#include <fstream>
#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
    Game game;
    game.gameStart();
    game.backGround();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    //게임이 완전히 끝날때까지 반복
    while (!game.checkAllSuccess())
    {
        //화면 지우기
        clear();
	game.backGround();
        //입력받아 움직이기
        int input = getch();
	//F1 = Quit
	if(input == KEY_F(1)) break;
        game.move(input);
    } 
    endwin();
}
