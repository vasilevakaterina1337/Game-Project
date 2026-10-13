#include "UnitFactory.h"
#include <memory>
#include <stdexcept>

using namespace std;

unique_ptr<Unit> InfantryFactory::createUnit(UnitType type, int x, int y, bool isEnemy) const {
    switch (type) {
    case UnitType::SWORDSMAN: return make_unique<Swordsman>(x, y, isEnemy);
    case UnitType::SPEARMAN:  return make_unique<Spearman>(x, y, isEnemy);
    default: throw invalid_argument("InfantryFactory не может создать этот тип");
    }
}

unique_ptr<Unit> ArcherFactory::createUnit(UnitType type, int x, int y, bool isEnemy) const {
    switch (type) {
    case UnitType::LIGHT_ARCHER: return make_unique<LightArcher>(x, y, isEnemy);
    case UnitType::HEAVY_ARCHER: return make_unique<HeavyArcher>(x, y, isEnemy);
    default: throw invalid_argument("ArcherFactory не может создать этот тип");
    }
}

unique_ptr<Unit> CavalryFactory::createUnit(UnitType type, int x, int y, bool isEnemy) const {
    switch (type) {
    case UnitType::CATAPHRACT:   return make_unique<Cataphract>(x, y, isEnemy);
    case UnitType::HORSE_ARCHER: return make_unique<HorseArcher>(x, y, isEnemy);
    default: throw invalid_argument("CavalryFactory не может создать этот тип");
    }
}