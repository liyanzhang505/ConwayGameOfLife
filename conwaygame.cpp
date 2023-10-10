#include "conwaygame.h"
#include <iostream>


ConwayGame::ConwayGame(int rows, int cols, BirthSurviveRule* bsRule) : GameBase(rows, cols, bsRule)
{
    initBuff();
}

void ConwayGame::evolve(int *grid)
{
    std::memset(pBuff, 0, rows * cols * sizeof(int));
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            bool flag = isAlive(grid, row, col);
            pBuff[row * cols + col] = static_cast<int>(flag);
        }
    }
    std::memcpy(grid, pBuff, rows * cols * sizeof(int));
}


void ConwayGame::initBuff() {
    pBuff = new int[rows * cols];
    memset(pBuff, 0, rows * cols * sizeof(int));
}

void ConwayGame::reset(int rows, int cols)
{
    delete[] pBuff;
    this->rows = rows;
    this->cols = cols;
    initBuff();

}

ConwayGame::~ConwayGame()
{
    delete[] pBuff;
}

bool ConwayGame::isAlive(int *grid, int row, int col)
{

    int aliveCount = 0;
    for (int i = std::max(row - 1, 0); i < std::min(row + 2, rows); i++)
    {
        for (int j = std::max(col - 1, 0); j < std::min(col + 2, cols); j++)
        {
//            std::cout << "i: " << i <<  " j" << j << std::endl;
            aliveCount += grid[i * cols + j];
        }
    }

    // minus its own state
    aliveCount -= grid[row * cols + col] & 1;


    if (grid[row * cols + col] == 1) {
        for (int surviveRule : bsRule->getSurviveSetting()) {
            if (surviveRule == aliveCount) {
                return true;
            }
        }
        return false;
    } else {
        for (int birthRule : bsRule->getBirthSetting()) {
            if (birthRule == aliveCount) {
                return true;
            }
        }
        return false;
    }

}
