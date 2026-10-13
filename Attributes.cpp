#include "Attributes.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

Health::Health(int max_val) : max_value_(max_val), value_(max_val) {
    if (max_val <= 0) throw invalid_argument("Максимальное здоровье должно быть положительным");
}

int Health::getValue() const { return value_; }
int Health::getMaxValue() const { return max_value_; }

void Health::decrease(int amount) {
    if (amount < 0) return;
    value_ = max(0, value_ - amount);
}

void Health::increase(int amount) {
    if (amount < 0) return;
    value_ = min(max_value_, value_ + amount);
}

bool Health::isZero() const { return value_ == 0; }

Armor::Armor(int val) : value_(max(0, val)) {}

int Armor::getValue() const { return value_; }

int Armor::reduceDamage(int incomingDamage) const {
    return max(0, incomingDamage - value_);
}

Attack::Attack(int val) : value_(max(0, val)) {}

int Attack::getValue() const { return value_; }

int Attack::calculateDamage(const Armor& targetArmor) const {
    return targetArmor.reduceDamage(value_);
}