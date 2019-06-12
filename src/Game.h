#include <vector>
#include <utility>
using namespace std;

class Game
{
private:
    int numOfStages, currentStage, stepCount, pushCount;
    vector<bool> finished;
    vector<vector<vector<int>>> maps;
    vector<pair<int, int>> playerPositions;
    vector<vector<pair<int, int>>> goalPositions;
    
public:
    int getStepCount() { return stepCount; }
    int getPushCount() { return pushCount; }
    int getCurrentStage() { return currentStage; }
    vector<bool> getFinised() { return finished; }
    vector<vector<vector<int>>> getMaps() { return maps; }

    void gameStart();
    void setMaps();
    void setPlayer();
    void move(const int direction);
    void walk(const int y, const int x, const int nextY, const int nextX);
    void push(const int y, const int x, const int nextY, const int nextX, const int afterY, const int afterX);
    bool checkGoal(const int y, const int x) const;
    void nextStage();
    bool checkSuccess() const;
    bool checkAllSuccess() const;
};
