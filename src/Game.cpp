#include <ncurses.h>
#include <fstream>
#include "Game.h"
using namespace std;

void Game::gameStart()
{
    setMaps();
    setPlayer();
    this->finished.assign(numOfStages, false);
    this->currentStage = 0;
    this->stepCount = 0;
    this->pushCount = 0;
}

//3차원 벡터에 maps.txt를 가져와 데이터를 저장하는 함수
void Game::setMaps()
{
    int numOfRows, numOfCols;
    ifstream is("maps.txt");
    is >> numOfStages;
    this->maps.assign(numOfStages, vector<vector<int>>());
    this->goalPositions.assign(numOfStages, vector<pair<int, int>>());
    for (int i = 0; i < numOfStages; i++)
    {
        is >> numOfRows >> numOfCols;
        this->maps[i].assign(numOfRows, vector<int>(numOfCols, -1));
        for (int j = 0; j < numOfRows; j++)
        {
            for (int k = 0; k < numOfCols; k++)
            {
                is >> this->maps[i][j][k];
                if (maps[i][j][k] == 3)
                    goalPositions[i].push_back(make_pair(j, k));
            }
        }
    }
}

//맵 위에 플레이어를 위치시킨다.
void Game::setPlayer()
{
    int y, x;
    ifstream is("PlayerPositions.txt");
    is >> numOfStages;
    for (int i = 0; i < numOfStages; i++)
    {
        is >> y >> x;
        playerPositions.push_back(make_pair(y, x));
    }

    for (int i = 0; i < numOfStages; i++)
    {
        for (int j = 0; j < maps[i].size(); j++)
        {
            for (int k = 0; k < maps[i][j].size(); k++)
            {
                if (j == playerPositions[i].first && k == playerPositions[i].second)
                    maps[i][j][k] = 5;
            }
        }
    }
}

void Game::move(const int direction)
{
    int y = playerPositions[currentStage].first;
    int x = playerPositions[currentStage].second;
    int nextY, nextX, afterY, afterX;
    if (direction == KEY_UP)
    {
        nextY = y - 1;
        nextX = x;
        afterY = y - 2;
        afterX = x;
    }
    else if (direction == KEY_DOWN)
    {
        nextY = y + 1;
        nextX = x;
        afterY = y + 2;
        afterX = x;
    }
    else if (direction == KEY_LEFT)
    {
        nextY = y;
        nextX = x - 1;
        afterY = y;
        afterX = x - 2;
    }
    else if (direction == KEY_RIGHT)
    {
        nextY = y;
        nextX = x + 1;
        afterY = y;
        afterX = x + 2;
    }
    else if (direction == KEY_F(2))
    {
	playerPositions.clear();
        setMaps();
	setPlayer();
	this->stepCount = 0;
        this->pushCount = 0;
    }



    //다음칸이 벽이 아닐 때
    if (maps[currentStage][nextY][nextX] != 1)
    {
        //다음칸이 비어있을 때 그냥 앞으로 전진
        if (maps[currentStage][nextY][nextX] == 0 || maps[currentStage][nextY][nextX] == 3)
            walk(y, x, nextY, nextX);
        //다음칸이 상자일 때
        else
            //다음다음칸이 빈칸일 경우 PUSH
            if (maps[currentStage][afterY][afterX] == 0 || maps[currentStage][afterY][afterX] == 3)
            push(y, x, nextY, nextX, afterY, afterX);

        //원래 있던 자리가 목적지인지 체크하여 변경
        if (checkGoal(y, x))
            maps[currentStage][y][x] = 3;
        //목적지에 모두 상자가 차있는지 체크
        if (checkSuccess())
            nextStage();
    }
}

//빈칸으로 한 칸 앞으로 가는 함수
void Game::walk(const int y, const int x, const int nextY, const int nextX)
{
    maps[currentStage][nextY][nextX] = 5;
    maps[currentStage][y][x] = 0;
    playerPositions[currentStage].first = nextY;
    playerPositions[currentStage].second = nextX;
    stepCount++;
}

//앞에 상자가 있을 경우 밀면서 한 칸 앞으로 가는 함수
void Game::push(const int y, const int x, const int nextY, const int nextX, const int afterY, const int afterX)
{
    maps[currentStage][afterY][afterX] = 2;
    maps[currentStage][nextY][nextX] = 5;
    maps[currentStage][y][x] = 0;
    playerPositions[currentStage].first = nextY;
    playerPositions[currentStage].second = nextX;
    stepCount++;
    pushCount++;
}

//다음 스테이지로 가는 함수
void Game::nextStage()
{
    finished[currentStage] = true;
    currentStage++;
    stepCount = 0;
    pushCount = 0;
}

//해당 좌표 위치가 목적지인지 체크하는 함수
bool Game::checkGoal(const int y, const int x) const
{
    for (int i = 0; i < goalPositions[currentStage].size(); i++)
        if (goalPositions[currentStage][i].first == y && goalPositions[currentStage][i].second == x)
            return true;
    return false;
}

//현재 스테이지가 완료되었는지 체크하는 함수
bool Game::checkSuccess() const
{
    int y, x;
    for (int i = 0; i < goalPositions[currentStage].size(); i++)
    {
        y = goalPositions[currentStage][i].first;
        x = goalPositions[currentStage][i].second;
        if (maps[currentStage][y][x] != 2)
            return false;
    }
    return true;
}

//모든 스테이지가 완료되었는지 체크하는 함수
bool Game::checkAllSuccess() const
{
    for (int i = 0; i < finished.size(); i++)
        if (finished[i] == false)
            return false;
    return true;
}
void Game::backGround(){
	initscr();
   	resize_term(30, 50);
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	bkgd(COLOR_PAIR(1));
	attron(COLOR_PAIR(1));
	border('*', '*', '*', '*', '*', '*', '*', '*');
	attroff(COLOR_PAIR(1));
	mvprintw(1, 20, "PUSHING");
	mvprintw(3, 8, "bbbbbbb     oooooo   xxx     xxx");
	mvprintw(4, 8, "bb    bb   ooo  ooo   xxx   xxx");
	mvprintw(5, 8, "bb    bb   oo    oo    xxx xxx");
	mvprintw(6, 8, "bbbbbbb    oo    oo     xxxxx");
	mvprintw(7, 8, "bb    bb   oo    oo    xxx xxx");
	mvprintw(8, 8, "bb    bb   ooo  ooo   xxx   xxx");
	mvprintw(9, 8, "bbbbbbb     oooooo   xxx     xxx");

	mvprintw(12, 12, "F1 = quit  F2= reset");

	//현재 발자국 수, 상자 민 횟수 그리기
        mvprintw(13, 12, "steps: %d", getStepCount());
        mvprintw(13, 23, "pushes: %d", getPushCount());

	//맵 그리기
 	for (int j = 0; j < getMaps()[getCurrentStage()].size(); j++)
        {
            for (int k = 0; k < getMaps()[getCurrentStage()][j].size(); k++)
            {
		if(getMaps()[getCurrentStage()][j][k] == 5)
			mvprintw(20 + j, 20 + k, "#");
		else if(getMaps()[getCurrentStage()][j][k] == 4)
			mvprintw(20 + j, 20 + k, " ");
		else
                mvprintw(20 + j, 20 + k, "%d", getMaps()[getCurrentStage()][j][k]);

            }
        }
	refresh();
}
