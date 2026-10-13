#pragma once
#include <memory>
#include "ConcreteUnits.h"

enum class UnitType {
    SWORDSMAN, SPEARMAN,
    LIGHT_ARCHER, HEAVY_ARCHER,
    CATAPHRACT, HORSE_ARCHER
};

class UnitFactory {
public:
    virtual ~UnitFactory() = default;
    virtual std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, bool isEnemy) const = 0;
};

class InfantryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, bool isEnemy) const override;
};

class ArcherFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, bool isEnemy) const override;
};

class CavalryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, bool isEnemy) const override;
};