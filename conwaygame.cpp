#include "conwaygame.h"
#include <iostream>
#include <random>
#include <ctime>


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

void ConwayGame::fullyAsyncEvolve(int *grid)
{
    std::default_random_engine engine(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distRow(0, rows - 1);
    std::uniform_int_distribution<int> distCol(0, cols - 1);

    int randRow = distRow(engine);
    int randCol = distCol(engine);

    bool flag = isAlive(grid, randRow, randCol);
    grid[randRow * cols + randCol] = static_cast<int>(flag);
}

void ConwayGame::partialSyncEvolve(int *grid, qreal syncRate)
{
    int totalCells = rows * cols;
    int cellsToUpdate = static_cast<int>(totalCells * syncRate);

    std::vector<int> cellIndices(totalCells);
    for(int i = 0; i < totalCells; ++i) {
        cellIndices[i] = i;
    }

    std::default_random_engine engine(static_cast<unsigned int>(std::time(nullptr)));

    std::shuffle(cellIndices.begin(), cellIndices.end(), engine);

    std::memcpy(pBuff, grid, rows * cols * sizeof(int));
    for(int i = 0; i < cellsToUpdate; ++i) {
        int index = cellIndices[i];
        int row = index / cols;
        int col = index % cols;
        bool flag = isAlive(grid, row, col);
        pBuff[row * cols + col] = static_cast<int>(flag);
    }
    std::memcpy(grid, pBuff, rows * cols * sizeof(int));
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
