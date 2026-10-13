#pragma once
#include <algorithm>
#include <stdexcept>

class Health {
private:
    int value_;
    int max_value_;
public:
    explicit Health(int max_val);
    int getValue() const;
    int getMaxValue() const;
    void decrease(int amount);
    void increase(int amount);
    bool isZero() const;
};

class Armor {
private:
    int value_;
public:
    explicit Armor(int val);
    int getValue() const;
    int reduceDamage(int incomingDamage) const;
};

class Attack {
private:
    int value_;
public:
    explicit Attack(int val);
    int getValue() const;
    int calculateDamage(const Armor& targetArmor) const;
};