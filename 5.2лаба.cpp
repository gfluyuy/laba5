#include <iostream>
#include <vector>
#include <windows.h>
#include <memory>
using namespace std;

// ==================== ЧАСТЬ 1: Перекрываемые методы и виртуальные функции ====================
class Base {
public:
    // Невиртуальный метод
    void novirtual_method() {
        cout << "не виртуальный метод класса Base" << endl;
    }

    // Виртуальный метод, можно переопределять в потомках
    virtual void virtual_method() {
        cout << "виртуальный метод класса Base" << endl;
    }

    // Метод вызывает невиртуальный метод
    void callsmethod1() {
        cout << "Вызван: ";
        novirtual_method();
    }

    // Метод вызывает виртуальный метод
    virtual void callsmethod2() {
        cout << "Вызван: ";
        virtual_method();
    }

    // Виртуальный деструктор для корректного удаления объектов потомков через указатель на базовый класс
    virtual ~Base() {
        cout << "Вызван деструктор Base" << endl;
    }
};

// Потомок Base
class Dec : public Base {
public:
    // Перекрытие невиртуального метода (скрывает метод Base)
    void novirtual_method() {
        cout << "перекрываемый не виртуальный метод класса Dec" << endl;
    }

    // Переопределение виртуального метода
    void virtual_method() override {
        cout << "переопределяемый виртуальный метод класса Dec" << endl;
    }
};

// ==================== ЧАСТЬ 2: Проверка типа и безопасное приведение ====================
class Base2 {
public:
    virtual string classname() { return "Base2"; } // возвращает имя класса
    virtual bool isA(const string& classname) { return classname == "Base2"; } // проверка типа
    virtual ~Base2() = default;
};

class Dec2 : public Base2 {
public:
    string classname() override { return "Dec2"; }
    bool isA(const string& classname) override {
        return classname == "Dec2" || Base2::isA(classname); // проверка иерархии классов
    }
};

// Ручное безопасное приведение типов без dynamic_cast
template <typename To, typename From>
To* manual_cast(From* obj) {
    if (obj && obj->isA(To().classname())) {
        return static_cast<To*>(obj); // безопасное приведение
    }
    return nullptr; // приведение невозможно
}

// ==================== ЧАСТЬ 3: Передача объектов в функции ====================
class Base3 {
public:
    Base3() { cout << "Конструктор Base3 по умолчанию" << endl; }
    Base3(Base3* obj) { cout << "Конструктор Base3 копирования указателей " << endl; }
    Base3(Base3& obj) { cout << "Конструктор Base3 копирования ссылок" << endl; }
    ~Base3() { cout << "Деструктор Base3" << endl; }
};

class Dec3 : public Base3 {
public:
    Dec3() { cout << "Конструктор Dec3 по умолчанию" << endl; }
    Dec3(Dec3* obj) { cout << "Конструктор Dec3 копирования указателей" << endl; }
    Dec3(Dec3& obj) { cout << "Конструктор Dec3 копирования ссылок" << endl; }
    ~Dec3() { cout << "Деструктор Dec3" << endl; }
};

// Функции для передачи объектов
void func1(Base3 obj) { cout << "func1 по значению" << endl; }
void func2(Base3* obj) { cout << "func2 по указателю" << endl; }
void func3(Base3& obj) { cout << "func3 по ссылке" << endl; }

// ==================== ЧАСТЬ 4: Возврат объектов из функций ====================
class Base4 {
public:
    Base4() { cout << "Конструктор Base4 по умолчанию" << endl; }
    Base4(const Base4&) { cout << "Конструктор Base4 копирования" << endl; }
    ~Base4() { cout << "Деструктор Base4" << endl; }
};

// Локальные и динамические объекты в функциях
Base4 func1_return() { Base4 locobj; return locobj; }           // локальный объект по значению
Base4* func2_return() { Base4* locobj = new Base4(); return locobj; } // локальный объект по указателю (динамический)
Base4& func3_return() { Base4* locobj = new Base4(); return *locobj; } // локальный объект по ссылке (опасно)
Base4 func4_return() { Base4* dynamicobj = new Base4(); return *dynamicobj; } // динамический объект по значению
Base4* func5_return() { Base4* dynamicobj = new Base4(); return dynamicobj; } // динамический объект по указателю
Base4& func6_return() { Base4* dynamicobj = new Base4(); return *dynamicobj; } // динамический объект по ссылке

// ==================== ЧАСТЬ 5: Умные указатели ====================
class toworkptr {
private:
    int item;
public:
    toworkptr(int item) : item(item) { cout << "toworkptr: " << item << " создан" << endl; }
    ~toworkptr() { cout << "toworkptr: " << item << " уничтожен" << endl; }
    void use() { cout << "Использование ресурса " << item << endl; }
};

// Функции для передачи умных указателей
void useUnique(unique_ptr<toworkptr> res) { if (res) res->use(); }
void useShared(shared_ptr<toworkptr> res) { if (res) res->use(); }

// ==================== MAIN ====================
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // -------------------- Прямой вызов объектов --------------------
    cout << "1.Прямой вызов объектов" << endl;
    Base base;
    Dec dec;
    cout << "Вызван ";
    base.novirtual_method(); // Base
    cout << "Вызван ";
    dec.novirtual_method();  // Dec

    // -------------------- Вызов через указатель --------------------
    cout << "2.Вызов объектов через указатель" << endl;
    Base* baseptr = &dec;
    cout << "Вызван ";
    baseptr->novirtual_method(); // Base, не виртуальный метод
    cout << "Вызван ";
    baseptr->virtual_method();   // Dec, виртуальный метод

    // -------------------- Вызовы методов внутри методов --------------------
    cout << "Вызовы методов внутри методов" << endl;
    Dec dec2;
    dec2.callsmethod1(); // вызывает невиртуальный метод
    dec2.callsmethod2(); // вызывает виртуальный метод

    // -------------------- Виртуальный деструктор --------------------
    cout << "Демострация виртуального деструктора" << endl;
    Base* newobject = new Dec();
    delete newobject; // вызывает деструктор Dec, затем Base

    // -------------------- Проверка типа --------------------
    cout << "3. Проверка типа через classname и isA" << endl;
    vector<Base2*> obj_list = { new Base2(), new Dec2() };
    for (auto& obj : obj_list) {
        cout << "classname: " << obj->classname();
        cout << ", isA('Dec2')? " << (obj->isA("Dec2") ? "true" : "false") << endl;
    }

    // -------------------- Ручное безопасное приведение --------------------
    cout << "4. Ручное безопасное приведение типов" << endl;
    for (auto& obj : obj_list) {
        Dec2* decPtr = manual_cast<Dec2>(obj); // ручное приведение
        if (decPtr)
            cout << "Объект Dec2, можно безопасно использовать методы Dec2" << endl;
        else
            cout << "Объект Dec2 не описан" << endl;
    }
    for (auto& obj : obj_list) delete obj;

    // -------------------- Передача объектов в функции --------------------
    cout << "5. Передача объектов Base3" << endl;
    Base3 base3;
    func1(base3);   // по значению
    func2(&base3);  // по указателю
    func3(base3);   // по ссылке

    cout << "6. Передача объектов Dec3" << endl;
    Dec3 dec3;
    func1(dec3);    // по значению
    func2(&dec3);   // по указателю
    func3(dec3);    // по ссылке

    // -------------------- Возврат объектов из функций --------------------
    cout << "7. Возврат объектов из функций" << endl;
    Base4 b1 = func1_return();
    Base4* b2 = func2_return();
    Base4& b3 = func3_return();
    Base4 b4 = func4_return();
    Base4* b5 = func5_return();
    delete b5;
    Base4& b6 = func6_return();

    // -------------------- Умные указатели --------------------
    cout << "8. unique_ptr" << endl;
    {
        unique_ptr<toworkptr> t1 = make_unique<toworkptr>(1);
        t1->use();
        unique_ptr<toworkptr> t2 = move(t1); // t1 становится пустым
        if (!t1) cout << "t1 теперь пуст" << endl;
    }

    cout << "9. shared_ptr" << endl;
    auto j1 = make_shared<toworkptr>(10);
    {
        auto j2 = j1; // счетчик увеличен
        cout << "Используется счётчик: " << j1.use_count() << endl;
    } // j2 уничтожен, счетчик уменьшается
    cout << "Используется счётчик: " << j1.use_count() << endl;

    auto j4 = make_shared<toworkptr>(40);
    useShared(j4);
    cout << "Используется счётчик после функции: " << j4.use_count() << endl;

    return 0;
}
