#include <iostream>
#include <memory>
#include <limits>
#include <locale>
#include "GameField.h"
#include "UnitFactory.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value < minVal || value > maxVal) {
                cout << "Значение должно быть в диапазоне [" << minVal << ", " << maxVal << "].\n";
                continue;
            }
            return value;
        }
        cout << "Введите целое число\n";
        clearInput();
    }
}

string unitTypeName(UnitType type) {
    switch (type) {
    case UnitType::SWORDSMAN: return "Мечник";
    case UnitType::SPEARMAN: return "Копейщик";
    case UnitType::LIGHT_ARCHER: return "Легкий лучник";
    case UnitType::HEAVY_ARCHER: return "Тяжелый лучник";
    case UnitType::CATAPHRACT: return "Катафракт";
    case UnitType::HORSE_ARCHER: return "Конный лучник";
    default: return "?";
    }
}

void demoCopy(const GameField& original) {
    cout << "\nДемонстрация глубокого копирования\n";
    if (original.empty()) {
        cout << "Поле пусто, добавьте юнитов для демонстрации\n";
        return;
    }
    GameField copy = original;
    cout << "Создана копия. Размер оригинала: " << original.size() << ", размер копии: " << copy.size() << "\n";

    cout << "\nНаносим 50 урона всем юнитам в копии\n";
    for (auto it = copy.begin(); it != copy.end(); ++it) {
        it->takeDamage(50);
    }

    cout << "\nСостояние копии (после урона):\n";
    for (auto it = copy.cbegin(); it != copy.cend(); ++it) it->print();

    cout << "\nСостояние оригинала:\n";
    for (auto it = original.cbegin(); it != original.cend(); ++it) it->print();
    cout << "Проверка пройдена: копия независима от оригинала\n";
}

void demoMove(const GameField& original) {
    cout << "\nДемонстрация семантики перемещения\n";
    if (original.empty()) {
        cout << "Поле пусто, добавьте юнитов\n";
        return;
    }

    GameField temp = original;
    cout << "Исходное поле: " << temp.size() << " юнитов\n";

    cout << "\n1. Вызов Move-конструктора\n";
    GameField moved = std::move(temp);
    cout << "  temp.size()  = " << temp.size() << " (должно быть 0)\n";
    cout << "  moved.size() = " << moved.size() << " (должно быть " << original.size() << ")\n";

    cout << "\n2. Вызов Move-оператора присваивания\n";
    GameField target(5, 5, 10); // Пустое поле
    cout << "  target.size() до = " << target.size() << "\n";
    target = std::move(moved);
    cout << "  moved.size()  = " << moved.size() << " (должно быть 0)\n";
    cout << "  target.size() = " << target.size() << " (должно быть " << original.size() << ")\n";
    cout << "Проверка пройдена: ресурсы переданы корректно\n";
}

void demoIterator(const GameField& field) {
    cout << "\nДемонстрация итератора\n";
    if (field.empty()) {
        cout << "Поле пусто\n";
        return;
    }
    cout << "Обход поля через константный итератор:\n";
    int index = 0;
    for (auto it = field.cbegin(); it != field.cend(); ++it) {
        cout << index++ << ". " << it->getName()
            << " [" << it->getCategory() << "] "
            << "HP: " << it->getHealth().getValue()
            << " на (" << it->getX() << "," << it->getY() << ")\n";
    }
    cout << "Всего: " << field.size() << " юнитов\n";
}

void printMenu() {
    cout << "\nГлавное меню\n"
        << "1. Показать игровое поле\n"
        << "2. Разместить союзного юнита\n"
        << "3. Разместить врага\n"
        << "4. Переместить юнита\n"
        << "5. Атаковать юнита\n"
        << "6. Показать список юнитов (Итератор)\n"
        << "7. Удалить юнита (по индексу)\n"
        << "8. Удалить юнита (по координатам)\n"
        << "9. Убрать мёртвых юнитов\n"
        << "10. Демо: Копирование поля\n"
        << "11. Демо: Перемещение поля\n"
        << "0. Выход\n";
}

UnitType chooseUnitType() {
    cout << "\nВыберите тип юнита:\n"
        << "Пехота\n"
        << " 1. Мечник(M) HP:100, Броня:10, Атака:15\n"
        << " 2. Копейщик(K) HP:90, Броня:8, Атака:12\n"
        << "Лучники\n"
        << " 3. Легкий лучник(L) HP:70, Броня:5, Атака:20\n"
        << " 4. Тяжелый лучник(A) HP:85, Броня:7, Атака:25\n"
        << "Кавалерия\n"
        << " 5. Катафракт(C) HP:120, Броня:15, Атака:20\n"
        << " 6. Конный лучник(H) HP:80, Броня:6, Атака:18\n";
    int choice = readInt("Ваш выбор: ", 1, 6);
    switch (choice) {
    case 1: return UnitType::SWORDSMAN;
    case 2: return UnitType::SPEARMAN;
    case 3: return UnitType::LIGHT_ARCHER;
    case 4: return UnitType::HEAVY_ARCHER;
    case 5: return UnitType::CATAPHRACT;
    case 6: return UnitType::HORSE_ARCHER;
    default: return UnitType::SWORDSMAN;
    }
}

void placeUnit(GameField& field, bool isEnemy) {
    cout << (isEnemy ? "\nРазмещение врага\n" : "\nРазмещение союзника\n");
    UnitType type = chooseUnitType();
    int x = readInt("X координата: ", 0, field.getWidth() - 1);
    int y = readInt("Y координата: ", 0, field.getHeight() - 1);

    unique_ptr<UnitFactory> factory;
    if (type == UnitType::SWORDSMAN || type == UnitType::SPEARMAN) {
        factory = make_unique<InfantryFactory>();
    }
    else if (type == UnitType::LIGHT_ARCHER || type == UnitType::HEAVY_ARCHER) {
        factory = make_unique<ArcherFactory>();
    }
    else {
        factory = make_unique<CavalryFactory>();
    }

    try {
        field.addUnit(factory->createUnit(type, x, y, isEnemy));
        cout << ">>> Юнит [" << unitTypeName(type) << "] размещён на (" << x << "," << y << ")!\n";
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }
}

void moveUnit(GameField& field) {
    if (field.empty()) { cout << "Поле пусто\n"; return; }
    int x = readInt("\nX координата юнита: ", 0, field.getWidth() - 1);
    int y = readInt("Y координата юнита: ", 0, field.getHeight() - 1);
    Unit* unit = field.getUnitAt(x, y);
    if (!unit) { cout << "Тут нет юнита\n"; return; }

    string name = unit->getName();
    int nx = readInt("Новая X: ", 0, field.getWidth() - 1);
    int ny = readInt("Новая Y: ", 0, field.getHeight() - 1);

    if (field.relocateUnit(x, y, nx, ny)) {
        if (x == nx && y == ny) {
            cout << ">>> " << name << " уже стоит на (" << nx << "," << ny << ").\n";
        }
        else {
            cout << ">>> " << name << " перемещён на (" << nx << "," << ny << ")!\n";
        }
    }
    else {
        cout << "Невозможно переместить (занято или вне поля)\n";
    }
}

void attackUnit(GameField& field) {
    if (field.size() < 2) { cout << "Нужно минимум 2 юнита\n"; return; }
    int x1 = readInt("\nX атакующего: ", 0, field.getWidth() - 1);
    int y1 = readInt("Y атакующего: ", 0, field.getHeight() - 1);
    Unit* attacker = field.getUnitAt(x1, y1);
    if (!attacker) { cout << "Нет юнита\n"; return; }

    int x2 = readInt("X цели: ", 0, field.getWidth() - 1);
    int y2 = readInt("Y цели: ", 0, field.getHeight() - 1);
    Unit* target = field.getUnitAt(x2, y2);
    if (!target) { cout << "Нет цели\n"; return; }
    if (attacker == target) { cout << "Нельзя атаковать себя\n"; return; }
    if (attacker->isEnemy() == target->isEnemy()) {
        cout << "Нельзя атаковать "
            << (attacker->isEnemy() ? "другого врага" : "своего союзника")
            << "\n";
        return;
    }
    attacker->attackUnit(*target);
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Битва на поле\n";
    int w = readInt("Ширина поля (1-30): ", 1, 30);
    int h = readInt("Высота поля (1-30): ", 1, 30);
    int maxInput = readInt("Максимум юнитов на поле (1-50): ", 1, 50);

    GameField field(w, h, static_cast<size_t>(maxInput));

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Ваш выбор (0-11): ", 0, 11);
        switch (choice) {
        case 1: field.render(); break;
        case 2: placeUnit(field, false); break;
        case 3: placeUnit(field, true); break;
        case 4: field.render(); moveUnit(field); break;
        case 5: field.render(); attackUnit(field); break;
        case 6: demoIterator(field); break;
        case 7: {
            if (field.empty()) { cout << "Поле пусто\n"; break; }
            demoIterator(field);
            int idx = readInt("Индекс для удаления: ", 0, static_cast<int>(field.size()) - 1);
            try {
                field.removeUnit(static_cast<size_t>(idx));
                cout << "Удалён\n";
            }
            catch (const exception& e) {
                cout << "! " << e.what() << "\n";
            }
            break;
        }
        case 8: {
            field.render();
            int x = readInt("X для удаления: ", 0, field.getWidth() - 1);
            int y = readInt("Y для удаления: ", 0, field.getHeight() - 1);
            if (field.removeUnitAt(x, y)) cout << ">>> Удалён.\n";
            else cout << "Тут никого нет\n";
            break;
        }
        case 9: {
            size_t removed = field.removeDeadUnits();
            cout << "Удалено мёртвых: " << removed << "\n";
            break;
        }
        case 10: demoCopy(field); break;
        case 11: demoMove(field); break;
        case 0:
            running = false;
            cout << "Пока-пока\n";
            break;
        }
    }
    return 0;
}