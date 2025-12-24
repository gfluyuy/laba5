#include <iostream>
#include <memory>  
#include <vector>
#include <string>  
using namespace std;

// Умные указатели unique_ptr и shared_ptr, их влияние на жизненный цикл объектов

class Resource {
public:
    string name;
    int id;

    Resource(string name, int id) : name(name), id(id) {
        cout << "Создан Resource: " << name << " (id=" << id << ")" << endl;
    }

    void use() {
        cout << "Используется Resource: " << name << " (id=" << id << ")" << endl;
    }

    // Виртуальный деструктор для полиморфизма
    virtual ~Resource() {
        cout << "Уничтожен Resource: " << name << " (id=" << id << ")" << endl;
    }
};

class ExtendedResource : public Resource {
public:
    double value;

    ExtendedResource(string name, int id, double value)
        : Resource(name, id), value(value) {
        cout << "Создан ExtendedResource со значением " << value << endl;
    }

    void extended_use() {
        cout << "ExtendedResource: " << name << ", значение=" << value << endl;
    }

    ~ExtendedResource() override {
        cout << "Уничтожен ExtendedResource" << endl;
    }
};

// Функции для демонстрации передачи умных указателей
void use_unique_ptr(unique_ptr<Resource> ptr) {
    cout << "use_unique_ptr: получил уникальный указатель" << endl;
    if (ptr) {
        ptr->use();
    }
    // ptr автоматически уничтожится при выходе из функции
}

void use_shared_ptr(shared_ptr<Resource> ptr) {
    cout << "use_shared_ptr: получил общий указатель" << endl;
    cout << "  Количество ссылок: " << ptr.use_count() << endl;
    if (ptr) {
        ptr->use();
    }
    // Счетчик ссылок уменьшается при выходе из функции
}

shared_ptr<Resource> create_and_return_shared() {
    cout << "create_and_return_shared: создаю новый Resource" << endl;
    auto ptr = make_shared<Resource>("Resource для возврата", 100);
    cout << "  Ссылок после создания: " << ptr.use_count() << endl;
    return ptr;
}

unique_ptr<Resource> create_and_return_unique() {
    cout << "create_and_return_unique: создаю новый Resource" << endl;
    auto ptr = make_unique<Resource>("Resource для уникального указателя", 200);
    return ptr;
}

void demo_unique_ptr() {
    cout << "\n=== Демонстрация unique_ptr ===" << endl;

    cout << "\n1. Создание unique_ptr:" << endl;
    unique_ptr<Resource> ptr1 = make_unique<Resource>("Ресурс 1", 1);
    ptr1->use();

    cout << "\n2. unique_ptr нельзя копировать, но можно перемещать:" << endl;
    // unique_ptr<Resource> ptr2 = ptr1; // ОШИБКА: нельзя копировать
    unique_ptr<Resource> ptr2 = move(ptr1); // Можно перемещать
    cout << "После move:" << endl;
    cout << "  ptr1: " << (ptr1 ? "указывает на объект" : "пустой указатель") << endl;
    cout << "  ptr2: ";
    if (ptr2) {
        ptr2->use();
    }

    cout << "\n3. Освобождение ресурса:" << endl;
    ptr2.reset();
    cout << "После reset:" << endl;
    cout << "  ptr2: " << (ptr2 ? "указывает на объект" : "пустой указатель") << endl;
}

void demo_shared_ptr() {
    cout << "\n=== Демонстрация shared_ptr ===" << endl;

    cout << "\n1. Создание shared_ptr:" << endl;
    shared_ptr<Resource> ptr1 = make_shared<Resource>("Общий ресурс 1", 10);
    cout << "  Количество ссылок: " << ptr1.use_count() << endl;

    cout << "\n2. shared_ptr можно копировать:" << endl;
    shared_ptr<Resource> ptr2 = ptr1;
    cout << "  После копирования:" << endl;
    cout << "  ptr1 ссылок: " << ptr1.use_count() << endl;
    cout << "  ptr2 ссылок: " << ptr2.use_count() << endl;

    cout << "\n3. Вложенный блок с копией:" << endl;
    {
        shared_ptr<Resource> ptr3 = ptr1;
        cout << "  Внутри блока:" << endl;
        cout << "  ptr1 ссылок: " << ptr1.use_count() << endl;
        cout << "  ptr3 ссылок: " << ptr3.use_count() << endl;
    }
    cout << "  После выхода из блока:" << endl;
    cout << "  ptr1 ссылок: " << ptr1.use_count() << endl;

    cout << "\n4. Сброс одного shared_ptr:" << endl;
    ptr2.reset();
    cout << "  После reset ptr2:" << endl;
    cout << "  ptr1 ссылок: " << ptr1.use_count() << endl;

    cout << "\n5. Уничтожение объекта при последнем shared_ptr:" << endl;
    ptr1.reset();
    cout << "  После reset ptr1:" << endl;
    cout << "  ptr1 ссылок: " << ptr1.use_count() << endl;
}

void demo_passing_pointers() {
    cout << "\n=== Передача умных указателей в функции ===" << endl;

    cout << "\n1. Передача unique_ptr (передача владения):" << endl;
    auto unique_ptr1 = make_unique<Resource>("Ресурс для передачи", 50);
    cout << "  До вызова функции:" << endl;
    cout << "  unique_ptr1: " << (unique_ptr1 ? "указывает на объект" : "пустой указатель") << endl;

    use_unique_ptr(move(unique_ptr1));

    cout << "  После вызова функции:" << endl;
    cout << "  unique_ptr1: " << (unique_ptr1 ? "указывает на объект" : "пустой указатель") << endl;

    cout << "\n2. Передача shared_ptr (разделение владения):" << endl;
    auto shared_ptr1 = make_shared<Resource>("Ресурс для общего доступа", 60);
    cout << "  До вызова функции:" << endl;
    cout << "  shared_ptr1 ссылок: " << shared_ptr1.use_count() << endl;

    use_shared_ptr(shared_ptr1);

    cout << "  После вызова функции:" << endl;
    cout << "  shared_ptr1 ссылок: " << shared_ptr1.use_count() << endl;
}

void demo_returning_pointers() {
    cout << "\n=== Возврат умных указателей из функций ===" << endl;

    cout << "\n1. Возврат shared_ptr:" << endl;
    {
        cout << "  Вызываем create_and_return_shared():" << endl;
        shared_ptr<Resource> ptr = create_and_return_shared();
        cout << "  После возврата:" << endl;
        cout << "  ptr ссылок: " << ptr.use_count() << endl;
    }
    cout << "  После выхода из блока объект удален" << endl;

    cout << "\n2. Возврат unique_ptr:" << endl;
    {
        cout << "  Вызываем create_and_return_unique():" << endl;
        unique_ptr<Resource> ptr = create_and_return_unique();
        cout << "  После возврата:" << endl;
        cout << "  ptr: " << (ptr ? "указывает на объект" : "пустой указатель") << endl;
    }
    cout << "  После выхода из блока объект удален" << endl;
}

void demo_polymorphism() {
    cout << "\n=== Работа с наследованием ===" << endl;

    cout << "\n1. unique_ptr с объектом-наследником:" << endl;
    unique_ptr<Resource> base_ptr = make_unique<ExtendedResource>(
        "Расширенный ресурс", 70, 3.14);

    base_ptr->use();

    // Для вызова методов ExtendedResource нужно привести тип
    ExtendedResource* derived_ptr = dynamic_cast<ExtendedResource*>(base_ptr.get());
    if (derived_ptr != nullptr) {
        derived_ptr->extended_use();
    }

    cout << "\n2. shared_ptr с объектом-наследником:" << endl;
    shared_ptr<Resource> shared_base_ptr = make_shared<ExtendedResource>(
        "Расширенный общий ресурс", 80, 2.71);

    cout << "  shared_base_ptr ссылок: " << shared_base_ptr.use_count() << endl;

    // Приведение типа для shared_ptr
    shared_ptr<ExtendedResource> shared_derived_ptr =
        dynamic_pointer_cast<ExtendedResource>(shared_base_ptr);

    if (shared_derived_ptr) {
        cout << "  Приведение успешно" << endl;
        shared_derived_ptr->extended_use();
        cout << "  shared_base_ptr ссылок: " << shared_base_ptr.use_count() << endl;
        cout << "  shared_derived_ptr ссылок: " << shared_derived_ptr.use_count() << endl;
    }
}

void demo_lifetime() {
    cout << "\n=== Время жизни объектов ===" << endl;

    cout << "\n1. Обычный локальный объект:" << endl;
    {
        cout << "  Начало блока" << endl;
        Resource local_obj("Локальный объект", 90);
        local_obj.use();
        cout << "  Конец блока" << endl;
    }
    cout << "  Объект удален" << endl;

    cout << "\n2. unique_ptr:" << endl;
    {
        cout << "  Начало блока" << endl;
        auto unique_ptr = make_unique<Resource>("Объект с unique_ptr", 91);
        unique_ptr->use();
        cout << "  Конец блока" << endl;
    }
    cout << "  Объект удален unique_ptr" << endl;

    cout << "\n3. shared_ptr с продлением жизни:" << endl;
    shared_ptr<Resource> external_ptr;
    {
        cout << "  Начало блока" << endl;
        auto shared_ptr = make_shared<Resource>("Объект с shared_ptr", 92);
        external_ptr = shared_ptr;
        cout << "  В блоке ссылок: " << shared_ptr.use_count() << endl;
        cout << "  Конец блока" << endl;
    }
    cout << "  После блока объект еще существует!" << endl;
    cout << "  external_ptr ссылок: " << external_ptr.use_count() << endl;
    cout << "  Сбрасываем external_ptr..." << endl;
    external_ptr.reset();
    cout << "  Объект удален" << endl;
}

void demo_collections() {
    cout << "\n=== Коллекции с умными указателями ===" << endl;

    cout << "\n1. Вектор unique_ptr:" << endl;
    vector<unique_ptr<Resource>> unique_vector;

    for (int i = 0; i < 3; i++) {
        unique_vector.push_back(
            make_unique<Resource>("Ресурс " + to_string(i), 100 + i)
        );
    }

    cout << "  Использование объектов:" << endl;
    for (auto& ptr : unique_vector) {
        ptr->use();
    }

    cout << "  Конец области видимости вектора" << endl;

    cout << "\n2. Вектор shared_ptr:" << endl;
    vector<shared_ptr<Resource>> shared_vector;

    auto shared1 = make_shared<Resource>("Общий ресурс A", 200);
    auto shared2 = make_shared<Resource>("Общий ресурс B", 201);

    shared_vector.push_back(shared1);
    shared_vector.push_back(shared2);
    shared_vector.push_back(shared1);

    cout << "  Количество ссылок:" << endl;
    cout << "  shared1 ссылок: " << shared1.use_count() << endl;
    cout << "  shared2 ссылок: " << shared2.use_count() << endl;

    cout << "  Использование объектов:" << endl;
    for (auto& ptr : shared_vector) {
        ptr->use();
    }

    cout << "  Конец области видимости вектора" << endl;
}

void demo_common_errors() {
    cout << "\n=== Типичные ошибки ===" << endl;

    cout << "\n1. Несколько unique_ptr на один объект:" << endl;
    Resource* raw_ptr = new Resource("Сырой указатель", 300);
    unique_ptr<Resource> ptr1(raw_ptr);
    cout << "  Создан unique_ptr на сырой указатель" << endl;
    // unique_ptr<Resource> ptr2(raw_ptr); // ОШИБКА: двойное удаление!

    cout << "\n2. Циклические ссылки с shared_ptr:" << endl;
    class Node {
    public:
        string name;
        shared_ptr<Node> next;

        Node(string name) : name(name) {
            cout << "  Создан узел: " << name << endl;
        }

        ~Node() {
            cout << "  Удален узел: " << name << endl;
        }
    };

    {
        cout << "  Создаем циклическую ссылку..." << endl;
        auto node1 = make_shared<Node>("Узел 1");
        auto node2 = make_shared<Node>("Узел 2");

        node1->next = node2;
        node2->next = node1;

        cout << "  node1 ссылок: " << node1.use_count() << endl;
        cout << "  node2 ссылок: " << node2.use_count() << endl;
    }
    cout << "  ПАМЯТЬ НЕ ОСВОБОЖДЕНА из-за циклической ссылки!" << endl;
}

void demo_weak_ptr() {
    cout << "\n=== weak_ptr для решения проблем ===" << endl;

    class Node {
    public:
        string name;
        weak_ptr<Node> next;

        Node(string name) : name(name) {
            cout << "  Создан узел: " << name << endl;
        }

        ~Node() {
            cout << "  Удален узел: " << name << endl;
        }

        void print_next() {
            shared_ptr<Node> next_ptr = next.lock();
            if (next_ptr) {
                cout << "  Следующий узел: " << next_ptr->name << endl;
            }
            else {
                cout << "  Следующего узла нет" << endl;
            }
        }
    };

    {
        cout << "  Создаем узлы с weak_ptr..." << endl;
        auto node1 = make_shared<Node>("Узел A");
        auto node2 = make_shared<Node>("Узел B");

        node1->next = node2;
        node2->next = node1;

        cout << "  node1 ссылок: " << node1.use_count() << endl;
        cout << "  node2 ссылок: " << node2.use_count() << endl;

        cout << "  Проверка ссылок:" << endl;
        node1->print_next();
        node2->print_next();
    }
    cout << "  ПАМЯТЬ ОСВОБОЖДЕНА КОРРЕКТНО!" << endl;
}

int part_4() {
    setlocale(LC_ALL, "Russian");

    cout << "Умные указатели unique_ptr и shared_ptr" << endl;
    cout << "==============================================" << endl;

    demo_unique_ptr();
    demo_shared_ptr();
    demo_passing_pointers();
    demo_returning_pointers();
    demo_polymorphism();
    demo_lifetime();
    demo_collections();
    demo_common_errors();
    demo_weak_ptr();

    cout << "\n==============================================" << endl;
    cout << "Демонстрация завершена" << endl;

    return 0;
}