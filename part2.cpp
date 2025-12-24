#include <iostream>
#include <string>
using namespace std;

// Здесь будет реализована проверка на принадлежность к какому-то классу,
// а также безопасное приведение типов через dynamic_cast и вручную

class FoodItem {
public:
    double massa;
    string type;
    string colour;
    int max_skorost;

    FoodItem() = default;

    FoodItem(double massa, string type, string colour, int max_skorost) {
        this->massa = massa;
        this->type = type;
        this->colour = colour;
        this->max_skorost = max_skorost;
        cout << "Создан FoodItem" << endl;
    }

    // виртуальный метод для проверки типа - classname() как требует лабораторная
    virtual string classname() const {
        return "FoodItem";
    }

    // виртуальный метод isA() как требует лабораторная
    virtual bool isA(const string& className) const {
        if (className == "FoodItem") {
            return true;
        }
        else {
            return false;
        }
    }

    virtual ~FoodItem() {
        cout << "Уничтожен FoodItem" << endl;
    }
};

class Fruit : public FoodItem {
public:
    int kolvo_chelovek_v_ekipage;
    int kalibr_orudiya;
    string model;

    Fruit(int kolvo_chelovek_v_ekipage, int kalibr_orudiya, string model)
        : FoodItem(45000, "Fruit", "green", 60)
    {
        this->kolvo_chelovek_v_ekipage = kolvo_chelovek_v_ekipage;
        this->kalibr_orudiya = kalibr_orudiya;
        this->model = model;
        cout << "Создан Fruit" << endl;
    }

    // переопределение виртуального метода classname()
    string classname() const override {
        return "Fruit";
    }

    // переопределение виртуального метода isA()
    bool isA(const string& className) const override {
        if (className == "Fruit") {
            return true;
        }
        else {
            return FoodItem::isA(className); // проверка вверх по иерархии
        }
    }

    // Специфический метод для фрукта
    void shoot() {
        cout << "Фрукт " << model << " съеден! Калибр: " << kalibr_orudiya << "мм" << endl;
    }

    ~Fruit() {
        cout << "Уничтожен Fruit" << endl;
    }
};

class Vegetable : public FoodItem {
public:
    int max_height;
    string vegetable_type;

    Vegetable(int max_height, string vegetable_type)
        : FoodItem(20000, "Vegetable", "gray", 800)
    {
        this->max_height = max_height;
        this->vegetable_type = vegetable_type;
        cout << "Создан Vegetable" << endl;
    }

    string classname() const override {
        return "Vegetable";
    }

    bool isA(const string& className) const override {
        if (className == "Vegetable") {
            return true;
        }
        else {
            return FoodItem::isA(className);
        }
    }

    void fly() {
        cout << "Овощ типа " << vegetable_type << " приготовлен на высоте " << max_height << "м" << endl;
    }

    ~Vegetable() {
        cout << "Уничтожен Vegetable" << endl;
    }
};

class ExoticFruit : public Vegetable {
public:
    int missile_count;

    ExoticFruit(int max_height, string vegetable_type, int missile_count)
        : Vegetable(max_height, vegetable_type)
    {
        this->missile_count = missile_count;
        cout << "Создан ExoticFruit" << endl;
    }

    string classname() const override {
        return "ExoticFruit";
    }

    bool isA(const string& className) const override {
        if (className == "ExoticFruit") {
            return true;
        }
        else {
            return Vegetable::isA(className);
        }
    }

    void launch_missile() {
        if (missile_count > 0) {
            cout << "Экзотический фрукт запускает ракету! Осталось: " << (missile_count - 1) << endl;
            missile_count--;
        }
        else {
            cout << "Нет ракет для запуска!" << endl;
        }
    }

    ~ExoticFruit() {
        cout << "Уничтожен ExoticFruit" << endl;
    }
};

void test_with_dynamic_cast_success() {
    cout << "=== Безопасное приведение типов через dynamic_cast (УСПЕХ) ===" << endl;
    FoodItem* machine = new Fruit(3, 125, "Яблоко");

    Fruit* tank_ptr = dynamic_cast<Fruit*>(machine);
    if (tank_ptr != nullptr) {
        cout << "Приведение успешно! Это Fruit." << endl;
        cout << "Численность экипажа: " << tank_ptr->kolvo_chelovek_v_ekipage << endl;
        cout << "Модель фрукта: " << tank_ptr->model << endl;
        cout << "Калибр орудия: " << tank_ptr->kalibr_orudiya << endl;
        tank_ptr->shoot(); // Вызов специфического метода
    }
    else {
        cout << "Приведение не удалось - это не Fruit" << endl;
    }

    delete machine;
}

void test_with_dynamic_cast_failure() {
    cout << endl << "=== Безопасное приведение типов через dynamic_cast (НЕУДАЧА) ===" << endl;

    // Создаем обычный FoodItem (не Fruit)
    FoodItem* machine = new FoodItem(1000, "Car", "red", 200);

    Fruit* tank_ptr = dynamic_cast<Fruit*>(machine);
    if (tank_ptr != nullptr) {
        cout << "Приведение успешно! Это Fruit." << endl;
    }
    else {
        cout << "Приведение НЕ удалось! Это НЕ Fruit, а " << machine->classname() << endl;
        // Не можем вызвать tank_ptr->shoot() - tank_ptr равен nullptr
    }

    delete machine;
}

void test_with_ruchnim_privedenie() {
    cout << endl << "=== Безопасное приведение типов вручную ===" << endl;

    FoodItem* machine = new Fruit(3, 125, "Манго");

    // Безопасная ручная проверка через наши методы classname() и isA()
    cout << "Проверка через classname(): ";
    if (machine->classname() == "Fruit") {
        cout << "classname подтверждает: это Fruit!" << endl;
        Fruit* tank_ptr = static_cast<Fruit*>(machine);
        cout << "Модель фрукта: " << tank_ptr->model << endl;
    }
    else {
        cout << "Это не Fruit" << endl;
    }

    cout << endl << "Проверка через isA(): ";
    if (machine->isA("Fruit")) {
        cout << "isA подтверждает: это Fruit!" << endl;
        Fruit* tank_ptr = static_cast<Fruit*>(machine);
        cout << "Калибр орудия: " << tank_ptr->kalibr_orudiya << endl;
    }
    else {
        cout << "Это не Fruit" << endl;
    }

    delete machine;
}

void test_prinaldlejnost_class() {
    cout << endl << "=== Проверка принадлежности классу ===" << endl;

    FoodItem* machine = new Fruit(4, 76, "Банан");

    // Способ 1: через dynamic_cast (корректный)
    cout << "Способ 1 - dynamic_cast: ";
    if (Fruit* tank = dynamic_cast<Fruit*>(machine)) {
        cout << "Объект принадлежит классу Fruit!" << endl;
    }
    else {
        cout << "Объект НЕ принадлежит классу Fruit" << endl;
    }

    // Способ 2: через classname()
    cout << "Способ 2 - classname(): " << machine->classname() << endl;

    // Способ 3: через isA()
    cout << "Способ 3 - isA(): ";
    if (machine->isA("Fruit")) {
        cout << "Это Fruit!" << endl;
    }

    if (machine->isA("FoodItem")) {
        cout << "Это также FoodItem!" << endl;
    }

    // Проверка на несуществующий класс
    if (!machine->isA("Vegetable")) {
        cout << "Это НЕ Vegetable!" << endl;
    }

    delete machine;
}

void demonstrate_dangerous_casting() {
    cout << endl << "=== Демонстрация ОПАСНОГО приведения типов ===" << endl;

    FoodItem* machine = new FoodItem(1000, "Car", "red", 200); // обычная машина, не фрукт!

    // ОПАСНОЕ приведение - static_cast без проверки
    Fruit* dangerous_tank = static_cast<Fruit*>(machine);
    cout << "Опасное приведение выполнено! Может привести к неопределенному поведению!" << endl;

    // Попытка доступа к полям Fruit!
    // cout << dangerous_tank->model << endl;
    // dangerous_tank->shoot();

    // Сравним с безопасным подходом через isA
    cout << "Проверка через isA: ";
    if (machine->isA("Fruit")) {
        cout << "Безопасно можно привести к Fruit" << endl;
    }
    else {
        cout << "ОБЪЕКТ НЕ ЯВЛЯЕТСЯ ФРУКТОМ! Приведение опасно!" << endl;
    }

    delete machine;
}

void demonstrate_problems_with_classname() {
    cout << endl << "=== Проблемы с методом classname() ===" << endl;

    FoodItem* tank = new Fruit(3, 120, "Апельсин");

    // Проблема 1: Легко ошибиться в написании строки
    cout << "Проблема 1: Чувствительность к регистру и опечаткам" << endl;
    if (tank->classname() == "Fruit") { // правильно
        cout << "  Правильное написание: работает" << endl;
    }

    if (tank->classname() == "fruit") { // ошибка в регистре
        cout << "  Неправильное написание (нижний регистр): не работает" << endl;
    }
    else {
        cout << "  Неправильное написание (нижний регистр): действительно не работает" << endl;
    }

    if (tank->classname() == "Fruitt") { // опечатка
        cout << "  Опечатка: не работает" << endl;
    }
    else {
        cout << "  Опечатка: действительно не работает" << endl;
    }

    // Проблема 2: Нет проверки всей иерархии
    cout << endl << "Проблема 2: classname() не проверяет иерархию" << endl;
    cout << "  classname() возвращает: " << tank->classname() << endl;
    cout << "  isA('FoodItem') проверяет всю иерархию: ";
    if (tank->isA("FoodItem")) {
        cout << "да" << endl;
    }
    else {
        cout << "нет" << endl;
    }

    delete tank;
}

void demonstrate_deep_hierarchy() {
    cout << endl << "=== иерархия классов===" << endl;

    ExoticFruit* jet = new ExoticFruit(15000, "Экзотический", 4);
    FoodItem* base_ptr = jet;

    cout << "1. Проверка через classname():" << endl;
    cout << "   jet->classname() = " << jet->classname() << endl;
    cout << "   base_ptr->classname() = " << base_ptr->classname() << endl;

    cout << endl << "2. Проверка через isA():" << endl;
    cout << "   jet->isA('ExoticFruit') = ";
    if (jet->isA("ExoticFruit")) {
        cout << "да" << endl;
    }
    else {
        cout << "нет" << endl;
    }

    cout << "   jet->isA('Vegetable') = ";
    if (jet->isA("Vegetable")) {
        cout << "да" << endl;
    }
    else {
        cout << "нет" << endl;
    }

    cout << "   jet->isA('FoodItem') = ";
    if (jet->isA("FoodItem")) {
        cout << "да" << endl;
    }
    else {
        cout << "нет" << endl;
    }

    cout << "   jet->isA('Fruit') = ";
    if (jet->isA("Fruit")) {
        cout << "да" << endl;
    }
    else {
        cout << "нет" << endl;
    }

    cout << endl << "3. dynamic_cast для глубокой иерархии:" << endl;
    Vegetable* aircraft_ptr = dynamic_cast<Vegetable*>(base_ptr);
    if (aircraft_ptr != nullptr) {
        cout << "   Приведение к Vegetable успешно!" << endl;
        aircraft_ptr->fly();
    }

    Fruit* tank_ptr = dynamic_cast<Fruit*>(base_ptr);
    if (tank_ptr != nullptr) {
        cout << "   Приведение к Fruit успешно!" << endl;
    }
    else {
        cout << "   Приведение к Fruit не удалось (ожидаемо)" << endl;
    }

    delete jet;
}

void demonstrate_need_for_type_check() {
    cout << endl << "=== Зачем нужна проверка типа? ===" << endl;

    // Создаем массив разнородных объектов
    FoodItem* machines[3];
    machines[0] = new Fruit(4, 120, "Яблоко");
    machines[1] = new Vegetable(10000, "Картофель");
    machines[2] = new ExoticFruit(18000, "Манго", 6);

    cout << "Обработка массива FoodItem*:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << endl << "Объект " << i << ": ";

        // Проверяем, является ли объект Fruit
        Fruit* tank = dynamic_cast<Fruit*>(machines[i]);
        if (tank != nullptr) {
            cout << "Это Fruit!" << endl;
            tank->shoot(); // Вызываем специфический метод
            continue;
        }

        // Проверяем, является ли объект Vegetable
        Vegetable* aircraft = dynamic_cast<Vegetable*>(machines[i]);
        if (aircraft != nullptr) {
            cout << "Это Vegetable!" << endl;
            aircraft->fly(); // Вызываем специфический метод
            continue;
        }

        // Проверяем, является ли объект ExoticFruit
        ExoticFruit* jet = dynamic_cast<ExoticFruit*>(machines[i]);
        if (jet != nullptr) {
            cout << "Это ExoticFruit!" << endl;
            jet->launch_missile(); // Вызываем специфический метод
            continue;
        }

        cout << "Неизвестный тип!" << endl;
    }

    // Очищаем память
    for (int i = 0; i < 3; i++) {
        delete machines[i];
    }
}

int part_2() {
    setlocale(LC_ALL, "Russian");

    test_with_dynamic_cast_success();
    test_with_dynamic_cast_failure();
    test_with_ruchnim_privedenie();
    test_prinaldlejnost_class();
    demonstrate_dangerous_casting();
    demonstrate_problems_with_classname();
    demonstrate_deep_hierarchy();
    demonstrate_need_for_type_check();

    return 0;
}