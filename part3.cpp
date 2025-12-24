#include <iostream>
#include <string>
using namespace std;

class Base {
public:
    int id;
    string name;

    Base() : id(0), name("Безымянный") {
        cout << "Создан Base по умолчанию" << endl;
    }

    Base(int id, string name) : id(id), name(name) {
        cout << "Создан Base: " << name << " (id=" << id << ")" << endl;
    }

    Base(const Base& other) : id(other.id), name(other.name) {
        cout << "Скопирован Base: " << name << endl;
    }

    virtual ~Base() {
        cout << "Удален Base: " << name << endl;
    }

    void print() const {
        cout << "Объект Base: id=" << id << ", имя='" << name << "'" << endl;
    }
};

class Desc : public Base {
public:
    double value;

    Desc() : Base(), value(0.0) {
        cout << "Создан Desc по умолчанию" << endl;
    }

    Desc(int id, string name, double value) : Base(id, name), value(value) {
        cout << "Создан Desc: " << name << ", значение=" << value << endl;
    }

    Desc(const Desc& other) : Base(other), value(other.value) {
        cout << "Скопирован Desc: " << name << endl;
    }

    ~Desc() {
        cout << "Удален Desc: " << name << endl;
    }

    void print() const {
        cout << "Объект Desc: id=" << id << ", имя='" << name << "', значение=" << value << endl;
    }
};

// Три способа передачи объектов
void func_by_value(Base obj) {
    cout << "В func_by_value: получил объект с id=" << obj.id << endl;
    obj.id = 100;
    cout << "В func_by_value: изменил id на " << obj.id << " (локально)" << endl;
}

void func_by_pointer(Base* obj) {
    cout << "В func_by_pointer: получил объект с id=" << obj->id << endl;
    obj->id = 200;
    cout << "В func_by_pointer: изменил id на " << obj->id << " (глобально)" << endl;
}

void func_by_reference(Base& obj) {
    cout << "В func_by_reference: получил объект с id=" << obj.id << endl;
    obj.id = 300;
    cout << "В func_by_reference: изменил id на " << obj.id << " (глобально)" << endl;
}

// Шесть способов возврата объектов
Base return_by_value_local() {
    Base local(1, "Локальный из return_by_value_local");
    cout << "return_by_value_local: возвращаю копию локального объекта" << endl;
    return local;
}

Base* return_pointer_to_local() {
    Base local(2, "Локальный из return_pointer_to_local");
    cout << "return_pointer_to_local: возвращаю указатель на локальный объект (ОПАСНО!)" << endl;
    return &local;
}

Base& return_reference_to_local() {
    Base local(3, "Локальный из return_reference_to_local");
    cout << "return_reference_to_local: возвращаю ссылку на локальный объект (ОПАСНО!)" << endl;
    return local;
}

Base return_copy_of_dynamic() {
    Base* dynamic_obj = new Base(4, "Динамический для копирования");
    cout << "return_copy_of_dynamic: возвращаю копию динамического объекта (утечка!)" << endl;
    return *dynamic_obj;
}

Base* return_pointer_to_dynamic() {
    Base* dynamic_obj = new Base(5, "Динамический из return_pointer_to_dynamic");
    cout << "return_pointer_to_dynamic: возвращаю указатель на динамический объект" << endl;
    return dynamic_obj;
}

Base& return_reference_to_dynamic() {
    Base* dynamic_obj = new Base(6, "Динамический из return_reference_to_dynamic");
    cout << "return_reference_to_dynamic: возвращаю ссылку на динамический объект" << endl;
    return *dynamic_obj;
}

void demo_passing() {
    cout << "\n=== Демонстрация передачи объектов ===" << endl;

    cout << "\n1. Передача Base тремя способами:" << endl;
    Base obj(10, "Тестовый Base");
    obj.print();

    cout << "\nа) Передача по значению:" << endl;
    func_by_value(obj);
    cout << "После func_by_value исходный объект: ";
    obj.print();

    cout << "\nб) Передача по указателю:" << endl;
    func_by_pointer(&obj);
    cout << "После func_by_pointer исходный объект: ";
    obj.print();

    cout << "\nв) Передача по ссылке:" << endl;
    func_by_reference(obj);
    cout << "После func_by_reference исходный объект: ";
    obj.print();

    cout << "\n2. Передача Desc (проверка срезки):" << endl;
    Desc desc_obj(20, "Тестовый Desc", 99.9);
    desc_obj.print();

    cout << "\nПередача Desc по значению (срезка):" << endl;
    func_by_value(desc_obj);
    cout << "Исходный Desc остался без изменений: ";
    desc_obj.print();
}

void demo_returning() {
    cout << "\n=== Демонстрация возврата объектов ===" << endl;

    cout << "\n1. Возврат локального объекта по значению:" << endl;
    {
        Base obj = return_by_value_local();
        cout << "Полученный объект: ";
        obj.print();
        cout << "Выход из блока" << endl;
    }

    cout << "\n2. Возврат указателя на локальный объект (опасно!):" << endl;
    {
        Base* ptr = return_pointer_to_local();
        cout << "Указатель получен, но объект уже уничтожен!" << endl;
        cout << "Использование указателя приведет к ошибке" << endl;
    }

    cout << "\n3. Возврат ссылки на локальный объект (опасно!):" << endl;
    {
        Base& ref = return_reference_to_local();
        cout << "Ссылка получена, но объект уже уничтожен!" << endl;
        cout << "Использование ссылки приведет к ошибке" << endl;
    }

    cout << "\n4. Возврат копии динамического объекта (утечка памяти!):" << endl;
    {
        Base obj = return_copy_of_dynamic();
        cout << "Получена копия: ";
        obj.print();
        cout << "Динамический объект не удален - утечка памяти!" << endl;
    }

    cout << "\n5. Возврат указателя на динамический объект:" << endl;
    {
        Base* ptr = return_pointer_to_dynamic();
        cout << "Получен указатель: ";
        ptr->print();
        delete ptr;
        cout << "Память освобождена корректно" << endl;
    }

    cout << "\n6. Возврат ссылки на динамический объект:" << endl;
    {
        Base& ref = return_reference_to_dynamic();
        cout << "Получена ссылка: ";
        ref.print();
        delete& ref;
        cout << "Память освобождена корректно" << endl;
    }
}

void demo_casting() {
    cout << "\n=== Демонстрация приведения типов ===" << endl;

    cout << "\n1. Создаем объект Desc:" << endl;
    Desc desc(30, "Производный объект", 50.5);
    desc.print();

    cout << "\n2. Работаем через указатель на Base:" << endl;
    Base* base_ptr = &desc;
    cout << "Указатель типа Base* указывает на Desc" << endl;

    cout << "\n3. Опасное приведение (static_cast без проверки):" << endl;
    Desc* static_ptr = static_cast<Desc*>(base_ptr);
    if (static_ptr) {
        cout << "static_cast выполнен успешно" << endl;
        static_ptr->print();
    }

    cout << "\n4. Безопасное приведение (dynamic_cast с проверкой):" << endl;
    Desc* dynamic_ptr = dynamic_cast<Desc*>(base_ptr);
    if (dynamic_ptr) {
        cout << "dynamic_cast: это действительно Desc!" << endl;
        dynamic_ptr->print();
    }
    else {
        cout << "dynamic_cast: приведение не удалось" << endl;
    }

    cout << "\n5. Попытка приведения обычного Base к Desc:" << endl;
    Base base_obj(40, "Обычный Base");
    Base* base_ptr2 = &base_obj;
    Desc* dynamic_ptr2 = dynamic_cast<Desc*>(base_ptr2);

    if (dynamic_ptr2) {
        cout << "Ошибка: Base был ошибочно приведен к Desc!" << endl;
    }
    else {
        cout << "Правильно: Base нельзя привести к Desc" << endl;
    }
}

int part_3() {
    setlocale(LC_ALL, "Russian");

    cout << "Передача и возврат объектов, жизненный цикл" << endl;
    cout << "==============================================" << endl;

    demo_passing();
    demo_returning();
    demo_casting();

    cout << "\n==============================================" << endl;
    cout << "Демонстрация завершена" << endl;

    return 0;
}