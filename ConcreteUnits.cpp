#include "ConcreteUnits.h"
#include <memory>
#include <string>

using namespace std;

Swordsman::Swordsman(int x, int y, bool enemy)
    : Infantry(x, y, "Мечник", 'M', 100, 10, 15, enemy) {
}
string Swordsman::getType() const { return "Мечник"; }
unique_ptr<Unit> Swordsman::clone() const { return make_unique<Swordsman>(*this); }

Spearman::Spearman(int x, int y, bool enemy)
    : Infantry(x, y, "Копейщик", 'K', 90, 8, 12, enemy) {
}
string Spearman::getType() const { return "Копейщик"; }
unique_ptr<Unit> Spearman::clone() const { return make_unique<Spearman>(*this); }

LightArcher::LightArcher(int x, int y, bool enemy)
    : Archer(x, y, "Легкий лучник", 'L', 70, 5, 20, enemy) {
}
string LightArcher::getType() const { return "Легкий лучник"; }
unique_ptr<Unit> LightArcher::clone() const { return make_unique<LightArcher>(*this); }

HeavyArcher::HeavyArcher(int x, int y, bool enemy)
    : Archer(x, y, "Тяжелый лучник", 'A', 85, 7, 25, enemy) {
}
string HeavyArcher::getType() const { return "Тяжелый лучник"; }
unique_ptr<Unit> HeavyArcher::clone() const { return make_unique<HeavyArcher>(*this); }

Cataphract::Cataphract(int x, int y, bool enemy)
    : Cavalry(x, y, "Катафракт", 'C', 120, 15, 20, enemy) {
}
string Cataphract::getType() const { return "Катафракт"; }
unique_ptr<Unit> Cataphract::clone() const { return make_unique<Cataphract>(*this); }

HorseArcher::HorseArcher(int x, int y, bool enemy)
    : Cavalry(x, y, "Конный лучник", 'H', 80, 6, 18, enemy) {
}
string HorseArcher::getType() const { return "Конный лучник"; }
unique_ptr<Unit> HorseArcher::clone() const { return make_unique<HorseArcher>(*this); }