#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "Unit.h"

class GameFieldIterator {
private:
    std::vector<std::unique_ptr<Unit>>* units_;
    size_t index_;
public:
    GameFieldIterator(std::vector<std::unique_ptr<Unit>>* units, size_t index);
    Unit& operator*() const;
    Unit* operator->() const;
    GameFieldIterator& operator++();
    GameFieldIterator operator++(int);
    bool operator==(const GameFieldIterator& other) const;
    bool operator!=(const GameFieldIterator& other) const;
};

class GameFieldConstIterator {
private:
    const std::vector<std::unique_ptr<Unit>>* units_;
    size_t index_;
public:
    GameFieldConstIterator(const std::vector<std::unique_ptr<Unit>>* units, size_t index);
    const Unit& operator*() const;
    const Unit* operator->() const;
    GameFieldConstIterator& operator++();
    GameFieldConstIterator operator++(int);
    bool operator==(const GameFieldConstIterator& other) const;
    bool operator!=(const GameFieldConstIterator& other) const;
};

class GameField {
private:
    int width_;
    int height_;
    size_t max_units_;
    std::vector<std::unique_ptr<Unit>> units_;
    std::unordered_map<unsigned long long, Unit*> coordIndex_;

    static unsigned long long key(int x, int y);
    void rebuildIndex();

public:
    GameField(int w, int h, size_t max_units = 100);
    ~GameField() = default;

    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;

    int getWidth() const;
    int getHeight() const;
    size_t getMaxUnits() const;
    size_t size() const;
    bool empty() const;

    void addUnit(std::unique_ptr<Unit> unit);
    void removeUnit(size_t index);
    bool removeUnitAt(int x, int y);
    bool relocateUnit(int fromX, int fromY, int toX, int toY);
    size_t removeDeadUnits();
    Unit* getUnit(size_t index) const;
    Unit* getUnitAt(int x, int y) const;
    void clear();

    GameFieldIterator begin();
    GameFieldIterator end();
    GameFieldConstIterator cbegin() const;
    GameFieldConstIterator cend() const;

    void render() const;
};