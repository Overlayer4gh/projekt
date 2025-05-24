#pragma once
class Eq
{

public:
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int attackPower;
    int defense;

   Eq(int hp = 100, int mana = 50, int attackPower = 10, int defense = 5)
        : hp(hp), maxHp(hp), mana(mana), maxMana(mana), attackPower(attackPower), defense(defense) {
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    void heal(int amount) {
        hp += amount;
        if (hp > maxHp) hp = maxHp;
    }

    bool useMana(int amount) {
        if (mana >= amount) {
            mana -= amount;
            return true;
        }
        return false;
    }

    void restoreMana(int amount) {
        mana += amount;
        if (mana > maxMana) mana = maxMana;
    };

};
