#ifndef GAMEBASE_H
#define GAMEBASE_H
#include "BirthSurviveRule.h"
#include <QtCore/qglobal.h>

class GameBase
{
protected:
    int rows, cols;
    int* pBuff;
    BirthSurviveRule* bsRule;
public:
    GameBase(int rows, int cols, BirthSurviveRule* bsRule): rows(rows), cols(cols), bsRule(bsRule) {}
    virtual void reset(int rows, int cols) = 0;
    virtual ~GameBase() = default;
    virtual void evolve(int* grid) = 0;
    virtual void fullyAsyncEvolve(int* grid) = 0;
    virtual void partialSyncEvolve(int* grid, qreal percent) = 0;
    std::vector<int> getBirthSetting() { return bsRule->getBirthSetting(); };
    std::vector<int> getSurviveSetting() { return bsRule->getSurviveSetting(); };
};

#endif // GAMEBASE_H
