#include "Unit.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

Unit::Unit(int x, int y, string name, char symbol,
    int hp, int armor, int attack, bool isEnemy)
    : x_(x), y_(y), name_(std::move(name)), symbol_(symbol), 
    health_(hp), armor_(armor), attack_(attack), isEnemy_(isEnemy) {
}

int Unit::getX() const { return x_; }
int Unit::getY() const { return y_; }
string Unit::getName() const { return name_; }
char Unit::getSymbol() const { return symbol_; }
bool Unit::isEnemy() const { return isEnemy_; }
const Health& Unit::getHealth() const { return health_; }
const Armor& Unit::getArmor() const { return armor_; }
const Attack& Unit::getAttack() const { return attack_; }

void Unit::takeDamage(int amount) { health_.decrease(amount); }
void Unit::heal(int amount) { health_.increase(amount); }
bool Unit::isDead() const { return health_.isZero(); }
void Unit::move(int new_x, int new_y) { x_ = new_x; y_ = new_y; }

void Unit::attackUnit(Unit& target) {
    int rawDamage = attack_.calculateDamage(target.getArmor());
    int actualDamage = min(rawDamage, target.getHealth().getValue());
    target.takeDamage(rawDamage);
    cout << name_ << " атакует " << target.getName()
        << " и наносит " << actualDamage << " урона"
        << " (У " << target.getName() << " осталось "
        << target.getHealth().getValue() << " HP)\n";
    if (target.isDead()) {
        cout << ">>> " << target.getName() << " повержен <<<\n";
    }
}

void Unit::print() const {
    string side = isEnemy_ ? "[Враг]" : "[Союзник]";
    cout << side << " [" << getCategory() << ":" << getType() << "] " << name_
        << " (HP:" << health_.getValue() << "/" << health_.getMaxValue()
        << ", Броня:" << armor_.getValue()
        << ", Атака:" << attack_.getValue() << ")"
        << " на (" << x_ << "," << y_ << ")\n";
}