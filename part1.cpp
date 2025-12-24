#include <iostream>
using namespace std;

// ==================== ЧАСТЬ 1: Кошки ====================
class Cat {
public:
    string klichka;
    string sound;
    int age;

    Cat() = default;

    Cat(string klichka, string sound, int age) {
        this->klichka = klichka;
        this->sound = sound;
        this->age = age;
        cout << "Вызван конструктор Cat с параметрами" << endl;
    }

    Cat(const Cat& other) {
        this->klichka = other.klichka;
        this->sound = other.sound;
        this->age = other.age;
        cout << "Вызван конструктор копирования Cat" << endl;
    }

    // БЕЗ virtual - перекрываемый метод
    void lai() {
        cout << "Был вызван метод базового класса!" << endl;
        cout << "Кошка по имени " << klichka << " сказала: " << sound << " !" << endl;
    }

    // С virtual - виртуальный метод
    virtual void happy_birthday() {
        cout << "Был вызван метод базового класса!" << endl;
        cout << "Ура! Нашей кошке по имени " << klichka << " исполнилось целых " << age << " !" << endl;
    }

    // НЕВИРТУАЛЬНЫЙ метод, который будет вызываться из других методов
    void eat() {
        cout << "Cat::eat() - Кошка ест" << endl;
    }

    // ВИРТУАЛЬНЫЙ метод, который будет вызываться из других методов
    virtual void sleep() {
        cout << "Cat::sleep() - Кошка спит" << endl;
    }

    // Метод, который вызывает другие методы (для демонстрации)
    void complex_behavior() {
        cout << "Cat::complex_behavior запущен!" << endl;
        cout << "Вызов невиртуального метода eat(): ";
        eat();
        cout << "Вызов виртуального метода sleep(): ";
        sleep();
    }

    void method_with_nonvirtual_call() {
        cout << "Cat::method_with_nonvirtual_call - вызов eat(): ";
        eat();
    }

    void method_with_virtual_call() {
        cout << "Cat::method_with_virtual_call - вызов sleep(): ";
        sleep();
    }

    virtual ~Cat() {
        cout << "Вызван деструктор базового класса Cat!" << endl;
    }
};

class Kitten : public Cat {
public:
    Kitten(int age, string sound, string klichka) : Cat(klichka, sound, age) {
        cout << "Вызван конструктор Kitten с параметрами" << endl;
    }

    Kitten(const Kitten& other) : Cat(other) {
        cout << "Вызван конструктор копирования Kitten" << endl;
    }

    // ПЕРЕКРЫВАЕМ метод (без virtual в базовом)
    void lai() {
        cout << "Был вызван метод наследуемого класса! Он перекрывает метод производного класса!" << endl;
        cout << "Ого, наш котёнок сказал: " << sound << " !" << endl;
    }

    // ПЕРЕОПРЕДЕЛЯЕМ виртуальный метод
    void happy_birthday() override {
        cout << "Был вызван метод наследуемого класса! Он переопределяет метод производного класса!" << endl;
        cout << "Гип-гип ура! Нашему котёнку по имени " << klichka << " исполнилось целых " << age << " !" << endl;
    }

    // ПЕРЕКРЫВАЕМ невиртуальный метод
    void eat() {
        cout << "Kitten::eat() - Котёнок ест молоко" << endl;
    }

    // ПЕРЕОПРЕДЕЛЯЕМ виртуальный метод
    void sleep() override {
        cout << "Kitten::sleep() - Котёнок спит в корзинке" << endl;
    }

    ~Kitten() {
        cout << "Вызван деструктор наследуемого класса Kitten!" << endl;
    }
};

class Tiger : public Cat {
public:
    Tiger(string klichka, string sound, int age) : Cat(klichka, sound, age) {
        cout << "Вызван конструктор Tiger" << endl;
    }
};

// ==================== Функции демонстрации ====================

void demonstrateVirtualDestructor() {
    cout << "=== Виртуальный деструктор ===" << endl;
    Cat* cat = new Kitten(6, "Мяу", "Снежок");
    delete cat;
}

void demonstrateVirtualMethods() {
    cout << "=== Виртуальные методы ===" << endl;
    Cat* animal = new Kitten(3, "Мяу-мяу", "Барсик");
    animal->happy_birthday();
    delete animal;
}

void demonstratePerekrivanieMethods() {
    cout << "=== Перекрываемые методы (без virtual) ===" << endl;
    Cat cat("Мурка", "Мяу", 5);
    Kitten kitten(12, "Мяу-мяу", "Пушок");

    cout << "Прямой вызов:" << endl;
    cat.lai();
    kitten.lai();

    Cat* ptr = &kitten;
    cout << "Через указатель на базовый класс:" << endl;
    ptr->lai();
}

void demonstrateMethodCallsFromBase() {
    cout << "=== Вызов методов из базового класса ===" << endl;
    Cat* cat = new Kitten(2, "Мяу-мяу", "Котофей");

    cout << "Через указатель на базовый класс:" << endl;
    cat->complex_behavior();

    cout << endl << "Вызов method_with_nonvirtual_call:" << endl;
    cat->method_with_nonvirtual_call();

    cout << endl << "Вызов method_with_virtual_call:" << endl;
    cat->method_with_virtual_call();

    delete cat;
}

void demonstrateDifferentPointerTypes() {
    cout << "=== Разные типы указателей ===" << endl;
    Kitten kitten(1, "Мяу-мяу", "Малыш");
    Cat* base_ptr = &kitten;
    Kitten* derived_ptr = &kitten;

    cout << "Через указатель на базовый класс:" << endl;
    base_ptr->lai();

    cout << "Через указатель на производный класс:" << endl;
    derived_ptr->lai();

    cout << "Виртуальные методы работают одинаково:" << endl;
    base_ptr->happy_birthday();
    derived_ptr->happy_birthday();
}

void demonstrateConstructorsAndDestructorsOrder() {
    cout << "=== Порядок вызова конструкторов и деструкторов ===" << endl;
    cout << "1. Создание Kitten:" << endl;
    Kitten* kitten = new Kitten(2, "Мяу", "Пушок");
    cout << endl << "2. Удаление Kitten:" << endl;
    delete kitten;

    cout << endl << "3. Создание Tiger (только конструкторы Cat и Tiger):" << endl;
    Tiger* tiger = new Tiger("Шерхан", "Ррр", 7);
    cout << endl << "4. Удаление Tiger:" << endl;
    delete tiger;
}

void demonstrateInheritanceWithoutConstructors() {
    cout << "=== Наследование без конструкторов в потомке ===" << endl;
    cout << "При создании потомка всегда вызывается конструктор предка!" << endl;
    cout << "Если в потомке нет своего конструктора, используется конструктор по умолчанию предка" << endl;
    Cat* cat = new Cat("Просто кошка", "Мяу", 3);
    cout << "Создана кошка: " << cat->klichka << endl;
    delete cat;
}

void demonstratePointerToBaseWithDerivedObject() {
    cout << "=== Указатель на базовый класс с объектом-потомком ===" << endl;
    Cat* catPtr = new Kitten(4, "Мяу-мяу", "Мурзик");
    cout << "Можно вызывать все методы Cat:" << endl;
    catPtr->lai();
    catPtr->happy_birthday();
    catPtr->eat();
    catPtr->sleep();
    delete catPtr;
}

void demonstrateDifferentMethodTypes() {
    cout << "=== Разные типы методов и их вызовы ===" << endl;
    Kitten kitten(3, "Мяу", "Барсик");
    Cat* catPtr = &kitten;
    Kitten* kittenPtr = &kitten;

    cout << "1. Невиртуальный метод в базовом классе, перекрытый в потомке:" << endl;
    cout << "   Через Cat*: "; catPtr->eat();
    cout << "   Через Kitten*: "; kittenPtr->eat();

    cout << "2. Виртуальный метод в базовом классе, переопределенный в потомке:" << endl;
    cout << "   Через Cat*: "; catPtr->sleep();
    cout << "   Через Kitten*: "; kittenPtr->sleep();
}

int part_1() {
    setlocale(LC_ALL, "Russian");

    demonstratePerekrivanieMethods();
    cout << endl;
    demonstrateVirtualMethods();
    cout << endl;
    demonstrateVirtualDestructor();
    cout << endl;
    demonstrateMethodCallsFromBase();
    cout << endl;
    demonstrateDifferentPointerTypes();
    cout << endl;
    demonstrateConstructorsAndDestructorsOrder();
    cout << endl;
    demonstrateInheritanceWithoutConstructors();
    cout << endl;
    demonstratePointerToBaseWithDerivedObject();
    cout << endl;
    demonstrateDifferentMethodTypes();

    return 0;
}