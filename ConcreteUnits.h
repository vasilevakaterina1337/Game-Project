#pragma once
#include "Unit.h"

class Swordsman : public Infantry {
public:
    explicit Swordsman(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};

class Spearman : public Infantry {
public:
    explicit Spearman(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};

class LightArcher : public Archer {
public:
    explicit LightArcher(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};

class HeavyArcher : public Archer {
public:
    explicit HeavyArcher(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};

class Cataphract : public Cavalry {
public:
    explicit Cataphract(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};

class HorseArcher : public Cavalry {
public:
    explicit HorseArcher(int x, int y, bool enemy = false);
    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};