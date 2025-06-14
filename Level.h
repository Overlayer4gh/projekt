#pragma once
class Level
{
public:
    int number;
    int enemyCount;
    bool hasBoss;

    Level(int num)
        : number(num),
        enemyCount(num),
        hasBoss(num % 5 == 0) // co pi¹ty poziom boss
    {
    }
};

