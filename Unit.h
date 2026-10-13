#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "Attributes.h"

class Unit {
protected:
    int x_, y_;
    Health health_;
    Armor armor_;
    Attack attack_;
    std::string name_;      
    char symbol_;
    bool isEnemy_;

public:
    Unit(int x, int y, std::string name, char symbol,
        int hp, int armor, int attack, bool isEnemy = false);

    virtual ~Unit() = default;

    Unit(const Unit&) = default;
    Unit& operator=(const Unit&) = default;

    virtual std::string getType() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::unique_ptr<Unit> clone() const = 0;  

    int getX() const;
    int getY() const;
    std::string getName() const;  
    char getSymbol() const;
    bool isEnemy() const;
    const Health& getHealth() const;
    const Armor& getArmor() const;
    const Attack& getAttack() const;

    void takeDamage(int amount);
    void heal(int amount);
    bool isDead() const;
    void move(int new_x, int new_y);
    void attackUnit(Unit& target);
    virtual void print() const;
};

class Infantry : public Unit {
public:
    using Unit::Unit;
    std::string getCategory() const override { return "Пехота"; }
};

class Archer : public Unit {
public:
    using Unit::Unit;
    std::string getCategory() const override { return "Лучник"; }
};

class Cavalry : public Unit {
public:
    using Unit::Unit;
    std::string getCategory() const override { return "Конница"; }
};