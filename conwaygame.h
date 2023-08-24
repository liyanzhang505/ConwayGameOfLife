#ifndef CONWAYGAME_H
#define CONWAYGAME_H
#include "GameBase.h"


class ConwayGame: public GameBase
{
public:
    ConwayGame(int rows, int cols);
    void evolve(int* grid) override;
    void reset(int rows, int cols) override;
private:
    void initBuff();
    bool isAlive(int* grid, int row, int col);
};


#endif // CONWAYGAME_H
