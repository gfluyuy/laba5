#include <iostream>
#include <memory>  
#include <vector>
#include <string>  
using namespace std;

// Óìíûå óêàçàòåëè unique_ptr è shared_ptr, èõ âëèÿíèå íà æèçíåííûé öèêë îáúåêòîâ

class Resource {
public:
    string name;
    int id;

    Resource(string name, int id) : name(name), id(id) {
        cout << "Ñîçäàí Resource: " << name << " (id=" << id << ")" << endl;
    }

    void use() {
        cout << "Èñïîëüçóåòñÿ Resource: " << name << " (id=" << id << ")" << endl;
    }

    // Âèðòóàëüíûé äåñòðóêòîð äëÿ ïîëèìîðôèçìà
    virtual ~Resource() {
        cout << "Óíè÷òîæåí Resource: " << name << " (id=" << id << ")" << endl;
    }
};

class ExtendedResource : public Resource {
public:
    double value;

    ExtendedResource(string name, int id, double value)
        : Resource(name, id), value(value) {
        cout << "Ñîçäàí ExtendedResource ñî çíà÷åíèåì " << value << endl;
    }

    void extended_use() {
        cout << "ExtendedResource: " << name << ", çíà÷åíèå=" << value << endl;
    }

    ~ExtendedResource() override {
        cout << "Óíè÷òîæåí ExtendedResource" << endl;
    }
};

// Ôóíêöèè äëÿ äåìîíñòðàöèè ïåðåäà÷è óìíûõ óêàçàòåëåé
void use_unique_ptr(unique_ptr<Resource> ptr) {
    cout << "use_unique_ptr: ïîëó÷èë óíèêàëüíûé óêàçàòåëü" << endl;
    if (ptr) {
        ptr->use();
    }
    // ptr àâòîìàòè÷åñêè óíè÷òîæèòñÿ ïðè âûõîäå èç ôóíêöèè
}

void use_shared_ptr(shared_ptr<Resource> ptr) {
    cout << "use_shared_ptr: ïîëó÷èë îáùèé óêàçàòåëü" << endl;
    cout << "  Êîëè÷åñòâî ññûëîê: " << ptr.use_count() << endl;
    if (ptr) {
        ptr->use();
    }
    // Ñ÷åò÷èê ññûëîê óìåíüøàåòñÿ ïðè âûõîäå èç ôóíêöèè
}

shared_ptr<Resource> create_and_return_shared() {
    cout << "create_and_return_shared: ñîçäàþ íîâûé Resource" << endl;
    auto ptr = make_shared<Resource>("Resource äëÿ âîçâðàòà", 100);
    cout << "  Ññûëîê ïîñëå ñîçäàíèÿ: " << ptr.use_count() << endl;
    return ptr;
}

unique_ptr<Resource> create_and_return_unique() {
    cout << "create_and_return_unique: ñîçäàþ íîâûé Resource" << endl;
    auto ptr = make_unique<Resource>("Resource äëÿ óíèêàëüíîãî óêàçàòåëÿ", 200);
    return ptr;
}

void demo_unique_ptr() {
    cout << "\n=== Äåìîíñòðàöèÿ unique_ptr ===" << endl;

    cout << "\n1. Ñîçäàíèå unique_ptr:" << endl;
    unique_ptr<Resource> ptr1 = make_unique<Resource>("Ðåñóðñ 1", 1);
    ptr1->use();

    cout << "\n2. unique_ptr íåëüçÿ êîïèðîâàòü, íî ìîæíî ïåðåìåùàòü:" << endl;
    // unique_ptr<Resource> ptr2 = ptr1; // ÎØÈÁÊÀ: íåëüçÿ êîïèðîâàòü
    unique_ptr<Resource> ptr2 = move(ptr1); // Ìîæíî ïåðåìåùàòü
    cout << "Ïîñëå move:" << endl;
    cout << "  ptr1: " << (ptr1 ? "óêàçûâàåò íà îáúåêò" : "ïóñòîé óêàçàòåëü") << endl;
    cout << "  ptr2: ";
    if (ptr2) {
        ptr2->use();
    }

    cout << "\n3. Îñâîáîæäåíèå ðåñóðñà:" << endl;
    ptr2.reset();
    cout << "Ïîñëå reset:" << endl;
    cout << "  ptr2: " << (ptr2 ? "óêàçûâàåò íà îáúåêò" : "ïóñòîé óêàçàòåëü") << endl;
}

void demo_shared_ptr() {
    cout << "\n=== Äåìîíñòðàöèÿ shared_ptr ===" << endl;

    cout << "\n1. Ñîçäàíèå shared_ptr:" << endl;
    shared_ptr<Resource> ptr1 = make_shared<Resource>("Îáùèé ðåñóðñ 1", 10);
    cout << "  Êîëè÷åñòâî ññûëîê: " << ptr1.use_count() << endl;

    cout << "\n2. shared_ptr ìîæíî êîïèðîâàòü:" << endl;
    shared_ptr<Resource> ptr2 = ptr1;
    cout << "  Ïîñëå êîïèðîâàíèÿ:" << endl;
    cout << "  ptr1 ññûëîê: " << ptr1.use_count() << endl;
    cout << "  ptr2 ññûëîê: " << ptr2.use_count() << endl;

    cout << "\n3. Âëîæåííûé áëîê ñ êîïèåé:" << endl;
    {
        shared_ptr<Resource> ptr3 = ptr1;
        cout << "  Âíóòðè áëîêà:" << endl;
        cout << "  ptr1 ññûëîê: " << ptr1.use_count() << endl;
        cout << "  ptr3 ññûëîê: " << ptr3.use_count() << endl;
    }
    cout << "  Ïîñëå âûõîäà èç áëîêà:" << endl;
    cout << "  ptr1 ññûëîê: " << ptr1.use_count() << endl;

    cout << "\n4. Ñáðîñ îäíîãî shared_ptr:" << endl;
    ptr2.reset();
    cout << "  Ïîñëå reset ptr2:" << endl;
    cout << "  ptr1 ññûëîê: " << ptr1.use_count() << endl;

    cout << "\n5. Óíè÷òîæåíèå îáúåêòà ïðè ïîñëåäíåì shared_ptr:" << endl;
    ptr1.reset();
    cout << "  Ïîñëå reset ptr1:" << endl;
    cout << "  ptr1 ññûëîê: " << ptr1.use_count() << endl;
}

void demo_passing_pointers() {
    cout << "\n=== Ïåðåäà÷à óìíûõ óêàçàòåëåé â ôóíêöèè ===" << endl;

    cout << "\n1. Ïåðåäà÷à unique_ptr (ïåðåäà÷à âëàäåíèÿ):" << endl;
    auto unique_ptr1 = make_unique<Resource>("Ðåñóðñ äëÿ ïåðåäà÷è", 50);
    cout << "  Äî âûçîâà ôóíêöèè:" << endl;
    cout << "  unique_ptr1: " << (unique_ptr1 ? "óêàçûâàåò íà îáúåêò" : "ïóñòîé óêàçàòåëü") << endl;

    use_unique_ptr(move(unique_ptr1));

    cout << "  Ïîñëå âûçîâà ôóíêöèè:" << endl;
    cout << "  unique_ptr1: " << (unique_ptr1 ? "óêàçûâàåò íà îáúåêò" : "ïóñòîé óêàçàòåëü") << endl;

    cout << "\n2. Ïåðåäà÷à shared_ptr (ðàçäåëåíèå âëàäåíèÿ):" << endl;
    auto shared_ptr1 = make_shared<Resource>("Ðåñóðñ äëÿ îáùåãî äîñòóïà", 60);
    cout << "  Äî âûçîâà ôóíêöèè:" << endl;
    cout << "  shared_ptr1 ññûëîê: " << shared_ptr1.use_count() << endl;

    use_shared_ptr(shared_ptr1);

    cout << "  Ïîñëå âûçîâà ôóíêöèè:" << endl;
    cout << "  shared_ptr1 ññûëîê: " << shared_ptr1.use_count() << endl;
}

void demo_returning_pointers() {
    cout << "\n=== Âîçâðàò óìíûõ óêàçàòåëåé èç ôóíêöèé ===" << endl;

    cout << "\n1. Âîçâðàò shared_ptr:" << endl;
    {
        cout << "  Âûçûâàåì create_and_return_shared():" << endl;
        shared_ptr<Resource> ptr = create_and_return_shared();
        cout << "  Ïîñëå âîçâðàòà:" << endl;
        cout << "  ptr ññûëîê: " << ptr.use_count() << endl;
    }
    cout << "  Ïîñëå âûõîäà èç áëîêà îáúåêò óäàëåí" << endl;

    cout << "\n2. Âîçâðàò unique_ptr:" << endl;
    {
        cout << "  Âûçûâàåì create_and_return_unique():" << endl;
        unique_ptr<Resource> ptr = create_and_return_unique();
        cout << "  Ïîñëå âîçâðàòà:" << endl;
        cout << "  ptr: " << (ptr ? "óêàçûâàåò íà îáúåêò" : "ïóñòîé óêàçàòåëü") << endl;
    }
    cout << "  Ïîñëå âûõîäà èç áëîêà îáúåêò óäàëåí" << endl;
}

void demo_polymorphism() {
    cout << "\n=== Ðàáîòà ñ íàñëåäîâàíèåì ===" << endl;

    cout << "\n1. unique_ptr ñ îáúåêòîì-íàñëåäíèêîì:" << endl;
    unique_ptr<Resource> base_ptr = make_unique<ExtendedResource>(
        "Ðàñøèðåííûé ðåñóðñ", 70, 3.14);

    base_ptr->use();

    // Äëÿ âûçîâà ìåòîäîâ ExtendedResource íóæíî ïðèâåñòè òèï
    ExtendedResource* derived_ptr = dynamic_cast<ExtendedResource*>(base_ptr.get());
    if (derived_ptr != nullptr) {
        derived_ptr->extended_use();
    }

    cout << "\n2. shared_ptr ñ îáúåêòîì-íàñëåäíèêîì:" << endl;
    shared_ptr<Resource> shared_base_ptr = make_shared<ExtendedResource>(
        "Ðàñøèðåííûé îáùèé ðåñóðñ", 80, 2.71);

    cout << "  shared_base_ptr ññûëîê: " << shared_base_ptr.use_count() << endl;

    // Ïðèâåäåíèå òèïà äëÿ shared_ptr
    shared_ptr<ExtendedResource> shared_derived_ptr =
        dynamic_pointer_cast<ExtendedResource>(shared_base_ptr);

    if (shared_derived_ptr) {
        cout << "  Ïðèâåäåíèå óñïåøíî" << endl;
        shared_derived_ptr->extended_use();
        cout << "  shared_base_ptr ññûëîê: " << shared_base_ptr.use_count() << endl;
        cout << "  shared_derived_ptr ññûëîê: " << shared_derived_ptr.use_count() << endl;
    }
}

void demo_lifetime() {
    cout << "\n=== Âðåìÿ æèçíè îáúåêòîâ ===" << endl;

    cout << "\n1. Îáû÷íûé ëîêàëüíûé îáúåêò:" << endl;
    {
        cout << "  Íà÷àëî áëîêà" << endl;
        Resource local_obj("Ëîêàëüíûé îáúåêò", 90);
        local_obj.use();
        cout << "  Êîíåö áëîêà" << endl;
    }
    cout << "  Îáúåêò óäàëåí" << endl;

    cout << "\n2. unique_ptr:" << endl;
    {
        cout << "  Íà÷àëî áëîêà" << endl;
        auto unique_ptr = make_unique<Resource>("Îáúåêò ñ unique_ptr", 91);
        unique_ptr->use();
        cout << "  Êîíåö áëîêà" << endl;
    }
    cout << "  Îáúåêò óäàëåí unique_ptr" << endl;

    cout << "\n3. shared_ptr ñ ïðîäëåíèåì æèçíè:" << endl;
    shared_ptr<Resource> external_ptr;
    {
        cout << "  Íà÷àëî áëîêà" << endl;
        auto shared_ptr = make_shared<Resource>("Îáúåêò ñ shared_ptr", 92);
        external_ptr = shared_ptr;
        cout << "  Â áëîêå ññûëîê: " << shared_ptr.use_count() << endl;
        cout << "  Êîíåö áëîêà" << endl;
    }
    cout << "  Ïîñëå áëîêà îáúåêò åùå ñóùåñòâóåò!" << endl;
    cout << "  external_ptr ññûëîê: " << external_ptr.use_count() << endl;
    cout << "  Ñáðàñûâàåì external_ptr..." << endl;
    external_ptr.reset();
    cout << "  Îáúåêò óäàëåí" << endl;
}

void demo_collections() {
    cout << "\n=== Êîëëåêöèè ñ óìíûìè óêàçàòåëÿìè ===" << endl;

    cout << "\n1. Âåêòîð unique_ptr:" << endl;
    vector<unique_ptr<Resource>> unique_vector;

    for (int i = 0; i < 3; i++) {
        unique_vector.push_back(
            make_unique<Resource>("Ðåñóðñ " + to_string(i), 100 + i)
        );
    }

    cout << "  Èñïîëüçîâàíèå îáúåêòîâ:" << endl;
    for (auto& ptr : unique_vector) {
        ptr->use();
    }

    cout << "  Êîíåö îáëàñòè âèäèìîñòè âåêòîðà" << endl;

    cout << "\n2. Âåêòîð shared_ptr:" << endl;
    vector<shared_ptr<Resource>> shared_vector;

    auto shared1 = make_shared<Resource>("Îáùèé ðåñóðñ A", 200);
    auto shared2 = make_shared<Resource>("Îáùèé ðåñóðñ B", 201);

    shared_vector.push_back(shared1);
    shared_vector.push_back(shared2);
    shared_vector.push_back(shared1);

    cout << "  Êîëè÷åñòâî ññûëîê:" << endl;
    cout << "  shared1 ññûëîê: " << shared1.use_count() << endl;
    cout << "  shared2 ññûëîê: " << shared2.use_count() << endl;

    cout << "  Èñïîëüçîâàíèå îáúåêòîâ:" << endl;
    for (auto& ptr : shared_vector) {
        ptr->use();
    }

    cout << "  Êîíåö îáëàñòè âèäèìîñòè âåêòîðà" << endl;
}

void demo_common_errors() {
    cout << "\n=== Òèïè÷íûå îøèáêè ===" << endl;

    cout << "\n1. Íåñêîëüêî unique_ptr íà îäèí îáúåêò:" << endl;
    Resource* raw_ptr = new Resource("Ñûðîé óêàçàòåëü", 300);
    unique_ptr<Resource> ptr1(raw_ptr);
    cout << "  Ñîçäàí unique_ptr íà ñûðîé óêàçàòåëü" << endl;
    // unique_ptr<Resource> ptr2(raw_ptr); // ÎØÈÁÊÀ: äâîéíîå óäàëåíèå!

    cout << "\n2. Öèêëè÷åñêèå ññûëêè ñ shared_ptr:" << endl;
    class Node {
    public:
        string name;
        shared_ptr<Node> next;

        Node(string name) : name(name) {
            cout << "  Ñîçäàí óçåë: " << name << endl;
        }

        ~Node() {
            cout << "  Óäàëåí óçåë: " << name << endl;
        }
    };

    {
        cout << "  Ñîçäàåì öèêëè÷åñêóþ ññûëêó..." << endl;
        auto node1 = make_shared<Node>("Óçåë 1");
        auto node2 = make_shared<Node>("Óçåë 2");

        node1->next = node2;
        node2->next = node1;

        cout << "  node1 ññûëîê: " << node1.use_count() << endl;
        cout << "  node2 ññûëîê: " << node2.use_count() << endl;
    }
    cout << "  ÏÀÌßÒÜ ÍÅ ÎÑÂÎÁÎÆÄÅÍÀ èç-çà öèêëè÷åñêîé ññûëêè!" << endl;
}

void demo_weak_ptr() {
    cout << "\n=== weak_ptr äëÿ ðåøåíèÿ ïðîáëåì ===" << endl;

    class Node {
    public:
        string name;
        weak_ptr<Node> next;

        Node(string name) : name(name) {
            cout << "  Ñîçäàí óçåë: " << name << endl;
        }

        ~Node() {
            cout << "  Óäàëåí óçåë: " << name << endl;
        }

        void print_next() {
            shared_ptr<Node> next_ptr = next.lock();
            if (next_ptr) {
                cout << "  Ñëåäóþùèé óçåë: " << next_ptr->name << endl;
            }
            else {
                cout << "  Ñëåäóþùåãî óçëà íåò" << endl;
            }
        }
    };

    {
        cout << "  Ñîçäàåì óçëû ñ weak_ptr..." << endl;
        auto node1 = make_shared<Node>("Óçåë A");
        auto node2 = make_shared<Node>("Óçåë B");

        node1->next = node2;
        node2->next = node1;

        cout << "  node1 ññûëîê: " << node1.use_count() << endl;
        cout << "  node2 ññûëîê: " << node2.use_count() << endl;

        cout << "  Ïðîâåðêà ññûëîê:" << endl;
        node1->print_next();
        node2->print_next();
    }
    cout << "  ÏÀÌßÒÜ ÎÑÂÎÁÎÆÄÅÍÀ ÊÎÐÐÅÊÒÍÎ!" << endl;
}

int part_4() {
    setlocale(LC_ALL, "Russian");

    cout << "Óìíûå óêàçàòåëè unique_ptr è shared_ptr" << endl;
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
    cout << "Äåìîíñòðàöèÿ çàâåðøåíà" << endl;

    return 0;
}
