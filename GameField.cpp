#include "GameField.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cctype>

using namespace std;

GameFieldIterator::GameFieldIterator(vector<unique_ptr<Unit>>* units, size_t index)
    : units_(units), index_(index) {
}

Unit& GameFieldIterator::operator*() const {
    if (!units_ || index_ >= units_->size()) throw out_of_range("Итератор вне диапазона");
    return *(*units_)[index_];
}
Unit* GameFieldIterator::operator->() const {
    if (!units_ || index_ >= units_->size()) throw out_of_range("Итератор вне диапазона");
    return (*units_)[index_].get();
}
GameFieldIterator& GameFieldIterator::operator++() { ++index_; return *this; }
GameFieldIterator GameFieldIterator::operator++(int) { GameFieldIterator t = *this; ++index_; return t; }
bool GameFieldIterator::operator==(const GameFieldIterator& other) const { return units_ == other.units_ && index_ == other.index_; }
bool GameFieldIterator::operator!=(const GameFieldIterator& other) const { return !(*this == other); }

GameFieldConstIterator::GameFieldConstIterator(const vector<unique_ptr<Unit>>* units, size_t index)
    : units_(units), index_(index) {
}

const Unit& GameFieldConstIterator::operator*() const {
    if (!units_ || index_ >= units_->size()) throw out_of_range("Итератор вне диапазона");
    return *(*units_)[index_];
}
const Unit* GameFieldConstIterator::operator->() const {
    if (!units_ || index_ >= units_->size()) throw out_of_range("Итератор вне диапазона");
    return (*units_)[index_].get();
}
GameFieldConstIterator& GameFieldConstIterator::operator++() { ++index_; return *this; }
GameFieldConstIterator GameFieldConstIterator::operator++(int) { GameFieldConstIterator t = *this; ++index_; return t; }
bool GameFieldConstIterator::operator==(const GameFieldConstIterator& other) const { return units_ == other.units_ && index_ == other.index_; }
bool GameFieldConstIterator::operator!=(const GameFieldConstIterator& other) const { return !(*this == other); }

GameField::GameField(int w, int h, size_t max_units)
    : width_(w), height_(h), max_units_(max_units) {
    if (w <= 0 || h <= 0) throw invalid_argument("Размеры поля должны быть положительными");
    if (max_units == 0) throw invalid_argument("Максимум юнитов должен быть > 0");
}

unsigned long long GameField::key(int x, int y) {
    return (static_cast<unsigned long long>(static_cast<unsigned int>(x)) << 32)
        | static_cast<unsigned int>(y);
}

void GameField::rebuildIndex() {
    coordIndex_.clear();
    for (const auto& u : units_) {
        coordIndex_[key(u->getX(), u->getY())] = u.get();
    }
}

GameField::GameField(const GameField& other)
    : width_(other.width_), height_(other.height_), max_units_(other.max_units_) {
    units_.reserve(other.units_.size());
    for (const auto& unit : other.units_) units_.push_back(unit->clone());
    rebuildIndex();
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        GameField temp(other);
        swap(width_, temp.width_);
        swap(height_, temp.height_);
        swap(max_units_, temp.max_units_);
        swap(units_, temp.units_);
        swap(coordIndex_, temp.coordIndex_);
    }
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : width_(other.width_), height_(other.height_),
    max_units_(other.max_units_), units_(move(other.units_)),
    coordIndex_(move(other.coordIndex_)) {
    other.width_ = 0; other.height_ = 0; other.max_units_ = 0;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        max_units_ = other.max_units_;
        units_ = move(other.units_);
        coordIndex_ = move(other.coordIndex_);
        other.width_ = 0; other.height_ = 0; other.max_units_ = 0;
    }
    return *this;
}

int GameField::getWidth() const { return width_; }
int GameField::getHeight() const { return height_; }
size_t GameField::getMaxUnits() const { return max_units_; }
size_t GameField::size() const { return units_.size(); }
bool GameField::empty() const { return units_.empty(); }

void GameField::addUnit(unique_ptr<Unit> unit) {
    if (!unit) return;
    if (units_.size() >= max_units_)
        throw overflow_error("Достигнут лимит юнитов (" + to_string(max_units_) + ")");
    if (unit->getX() < 0 || unit->getX() >= width_ ||
        unit->getY() < 0 || unit->getY() >= height_)
        throw out_of_range("Координаты за пределами поля");
    if (getUnitAt(unit->getX(), unit->getY()))
        throw runtime_error("Клетка занята");

    coordIndex_[key(unit->getX(), unit->getY())] = unit.get();
    units_.push_back(move(unit));
}

void GameField::removeUnit(size_t index) {
    if (index >= units_.size()) throw out_of_range("Индекс вне диапазона");
    coordIndex_.erase(key(units_[index]->getX(), units_[index]->getY()));
    units_.erase(units_.begin() + index);
}

bool GameField::removeUnitAt(int x, int y) {
    auto it = find_if(units_.begin(), units_.end(),
        [x, y](const unique_ptr<Unit>& u) { return u->getX() == x && u->getY() == y; });
    if (it == units_.end()) return false;
    coordIndex_.erase(key((*it)->getX(), (*it)->getY()));
    units_.erase(it);
    return true;
}

bool GameField::relocateUnit(int fromX, int fromY, int toX, int toY) {
    if (toX < 0 || toX >= width_ || toY < 0 || toY >= height_) return false;
    if (fromX == toX && fromY == toY) return true;

    auto it = find_if(units_.begin(), units_.end(),
        [fromX, fromY](const unique_ptr<Unit>& u) { return u->getX() == fromX && u->getY() == fromY; });
    if (it == units_.end()) return false;
    if (getUnitAt(toX, toY)) return false;

    coordIndex_.erase(key(fromX, fromY));
    (*it)->move(toX, toY);
    coordIndex_[key(toX, toY)] = it->get();
    return true;
}

size_t GameField::removeDeadUnits() {
    size_t before = units_.size();
    auto newEnd = remove_if(units_.begin(), units_.end(),
        [this](const unique_ptr<Unit>& u) {
            if (u->isDead()) {
                coordIndex_.erase(key(u->getX(), u->getY()));
                return true;
            }
            return false;
        });
    units_.erase(newEnd, units_.end());
    return before - units_.size();
}

Unit* GameField::getUnit(size_t index) const {
    if (index >= units_.size()) throw out_of_range("Индекс вне диапазона");
    return units_[index].get();
}

Unit* GameField::getUnitAt(int x, int y) const {
    auto it = coordIndex_.find(key(x, y));
    return (it != coordIndex_.end()) ? it->second : nullptr;
}

void GameField::clear() {
    units_.clear();
    coordIndex_.clear();
}

GameFieldIterator GameField::begin() { return GameFieldIterator(&units_, 0); }
GameFieldIterator GameField::end() { return GameFieldIterator(&units_, units_.size()); }
GameFieldConstIterator GameField::cbegin() const { return GameFieldConstIterator(&units_, 0); }
GameFieldConstIterator GameField::cend() const { return GameFieldConstIterator(&units_, units_.size()); }

void GameField::render() const {
    for (int x = 0; x < width_ + 2; ++x) cout << "X ";
    cout << "\n";

    for (int y = 0; y < height_; ++y) {
        cout << "X ";
        for (int x = 0; x < width_; ++x) {
            Unit* u = getUnitAt(x, y);
            if (u) {
                char c = u->getSymbol();
                if (u->isEnemy()) c = static_cast<char>(tolower(c));
                cout << c << " ";
            }
            else {
                cout << "  ";
            }
        }
        cout << "X\n";
    }

    for (int x = 0; x < width_ + 2; ++x) cout << "X ";
    cout << "\n";

    cout << "Заглавные буквы — СОЮЗНИКИ, строчные — ВРАГИ.\n"
        << "M/m — Мечник,  K/k — Копейщик\n"
        << "L/l — Легкий лучник,  A/a — Тяжелый лучник\n"
        << "C/c — Катафракт, H/h — Конный лучник\n";
}