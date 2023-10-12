#include "rletools.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

bool RLETools::saveToRLE(const QString &filename, const int* pCells, int rows, int cols, const std::vector<int> &birthRules, const std::vector<int> &survivalRules) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    QString birthStr = "B";
    for (int rule : birthRules) {
        birthStr += QString::number(rule);
    }

    QString survivalStr = "S";
    for (int rule : survivalRules) {
        survivalStr += QString::number(rule);
    }

    out << "x = " << cols << ", y = " << rows << ", rule = " << birthStr << "/" << survivalStr << "\n";

    for (int i = 0; i < rows; ++i) {
        int count = 1;
        for (int j = 0; j < cols; ++j) {
            int cell = pCells[i * cols + j];
            if (j == cols - 1 || pCells[i * cols + j + 1] != cell) {
                if (cell == 1) {
                    if (count > 1) out << count;
                    out << "o";
                } else {
                    if (count > 1) out << count;
                    out << "b";
                }
                count = 1;
            } else {
                count++;
            }
        }
        out << "$\n";
    }
    out << "!";

    return true;
}

bool RLETools::parseRule(const QString &ruleStr, std::vector<int> &birthRules, std::vector<int> &survivalRules) {
    QStringList parts = ruleStr.split("/");
    if (parts.size() != 2) {
        return false;
    }

    QString birthStr = parts[0].mid(1);
    QString survivalStr = parts[1].mid(1);

    for (QChar ch : birthStr) {
        if (ch.isDigit()) {
            birthRules.push_back(ch.digitValue());
        }
    }

    for (QChar ch : survivalStr) {
        if (ch.isDigit()) {
            survivalRules.push_back(ch.digitValue());
        }
    }
    return true;
}


bool RLETools::loadFromRLE(const QString &filename, int* &pCells, int &rows, int &cols, std::vector<int> &birthRules, std::vector<int> &survivalRules) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString line;

    // Skip char #
    while (!in.atEnd()) {
        line = in.readLine();
        if (!line.startsWith("#")) break;
    }

    // parse x, y
    QStringList parts = line.split(", ");
    cols = parts[0].split("=")[1].toInt();
    rows = parts[1].split("=")[1].toInt();
    pCells = new int[rows * cols];
    memset(pCells, 0,  rows * cols * sizeof(int));

    QString ruleStr = parts[2].split("=")[1].trimmed();

    qDebug() << "load ruleStr: " << ruleStr;
    if (parseRule(ruleStr, birthRules, survivalRules) != true) {
        qDebug() << "load rule str failed.";
        return false;
    }

    int x = 0, y = 0;
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (line.startsWith("#")) continue;

        int repeat = 0;
        for (int i = 0; i < line.length(); ++i) {
            QChar ch = line[i];
            if (ch.isDigit()) {
                repeat = repeat * 10 + ch.digitValue();
            } else if (ch == 'o' || ch == 'b') {
                int cellValue = (ch == 'o') ? 1 : 0;
                repeat = (repeat == 0) ? 1 : repeat;
                while (repeat) {
                    pCells[y * cols + x] = cellValue;
                    x++;
                    repeat--;
                }
                repeat = 0;
            } else if (ch == '$') {
                y += (repeat == 0) ? 1 : repeat;
                x = 0;
                repeat = 0;
            } else if (ch == '!') {
                for (int i = 0; i < rows; i++) {
                     QString rowStr = "row" + QString::number(i) + ": ";
                    for (int j = 0; j < cols; j++) {
                        rowStr + QString::number(pCells[i * cols + j]) + " ";
                    }
                    qDebug() << rowStr;
                }
                return true;
            }
        }
    }

    return false;
}
