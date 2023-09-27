#ifndef CONWAYGAME_H
#define CONWAYGAME_H
#include "gameBase.h"



class ConwayGame: public GameBase
{

public:
    ConwayGame(int rows, int cols, BirthSurviveRule* bsRule = new BirthSurviveRule({3}, {2, 3}));
    void evolve(int* grid) override;
    void reset(int rows, int cols) override;
    ~ConwayGame();
private:
    void initBuff();
    bool isAlive(int* grid, int row, int col);
};


#endif // CONWAYGAME_H
