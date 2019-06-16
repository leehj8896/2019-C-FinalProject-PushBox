#include <ncurses.h>
#include <fstream>
#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
    initscr();
    resize_term(40, 48);
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    attron(COLOR_PAIR(1));

    Game game;
    game.gameStart();

    //게임이 완전히 끝날때까지 반복
    while (!game.checkAllSuccess())
    {
        border('*', '*', '*', '*', '*', '*', '*', '*');
        mvprintw(1, 20, "PUSHING");
        mvprintw(3, 8, "bbbbbbb     oooooo   xxx     xxx");
        mvprintw(4, 8, "bb    bb   ooo  ooo   xxx   xxx");
        mvprintw(5, 8, "bb    bb   oo    oo    xxx xxx");
        mvprintw(6, 8, "bbbbbbb    oo    oo     xxxxx");
        mvprintw(7, 8, "bb    bb   oo    oo    xxx xxx");
        mvprintw(8, 8, "bb    bb   ooo  ooo   xxx   xxx");
        mvprintw(9, 8, "bbbbbbb     oooooo   xxx     xxx");
        mvprintw(11, 12, "q = quit  r= reset");

        //현재 발자국 수, 상자 민 횟수 그리기
        mvprintw(12, 12, "steps: %d", game.getStepCount());
        mvprintw(12, 23, "pushes: %d", game.getPushCount());

        //맵 그리기
        for (int j = 0; j < game.getMaps()[game.getCurrentStage()].size(); j++)
        {
            for (int k = 0; k < game.getMaps()[game.getCurrentStage()][j].size(); k++)
            {
                if (game.getMaps()[game.getCurrentStage()][j][k] == 5)
                    mvprintw(14 + j, 14 + k, "#");
                else if (game.getMaps()[game.getCurrentStage()][j][k] == 4)
                    mvprintw(14 + j, 14 + k, " ");
                else
                    mvprintw(14 + j, 14 + k, "%d", game.getMaps()[game.getCurrentStage()][j][k]);
            }
        }
        refresh();

        //입력받아 움직이기
        int input = getch();

        if (input == 'q')
            break;
        else if(input == 'r')
            game.reset();
        else
            game.move(input);

        //화면 지우기
        clear();
    }
    attroff(COLOR_PAIR(1));
    endwin();
}