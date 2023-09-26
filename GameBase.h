#ifndef GAMEBASE_H
#define GAMEBASE_H
#include "BirthSurviveRule.h"

class GameBase
{
protected:
    int rows, cols;
    int* pBuff;
    BirthSurviveRule bsRule;
public:
    GameBase(int rows, int cols, BirthSurviveRule bsRule): rows(rows), cols(cols), bsRule(bsRule) {}
    virtual void reset(int rows, int cols) = 0;
    virtual ~GameBase() = default;
    virtual void evolve(int* grid) = 0;
};

#endif // GAMEBASE_H
