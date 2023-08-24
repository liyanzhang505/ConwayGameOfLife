#ifndef GAMEBASE_H
#define GAMEBASE_H

class GameBase
{
protected:
    int rows, cols;
    int* pBuff;
public:
    GameBase(int rows, int cols): rows(rows), cols(cols) {}
    virtual void reset(int rows, int cols) = 0;
    virtual ~GameBase() = default;
    virtual void evolve(int* grid) = 0;
};

#endif // GAMEBASE_H
