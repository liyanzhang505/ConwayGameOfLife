#include "conwaygame.h"
#include <iostream>

ConwayGame::ConwayGame(int rows, int cols): GameBase(rows, cols)
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

bool ConwayGame::isAlive(int *grid, int row, int col)
{

    int aliveCount = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = col - 1; j <= col + 1; j++)
        {
            aliveCount += grid[i * cols + j];
        }
    }

    // minus its own state
    aliveCount -= grid[row * cols + col] & 1;

    if ((3 == aliveCount) || ((2 == aliveCount) && (1 == grid[row * cols +  col])))
    {
        return true;
    }
    else
    {
        return false;
    }
}
