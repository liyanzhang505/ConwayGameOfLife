#ifndef RLETOOLS_H
#define RLETOOLS_H

#include <QString>
#include <vector>

class RLETools
{
public:
    RLETools();
    static bool saveToRLE(const QString &filename, const int* pCells, int rows, int cols, const std::vector<int> &birthRules, const std::vector<int> &survivalRules);
    static bool loadFromRLE(const QString &filename, int* &pCells, int &rows, int &cols, std::vector<int> &birthRules, std::vector<int> &survivalRules);
    static bool parseRule(const QString &ruleStr, std::vector<int> &birthRules, std::vector<int> &survivalRules);
};

#endif // RLETOOLS_H
