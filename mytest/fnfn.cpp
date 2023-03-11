#include <functional>
#include <iostream>

using namespace std;
struct emitter {
    string str;

    emitter(const string& s) : str(s) { cout << "Created " << str << endl; }
    ~emitter() { cout << "Destroyed " << str << endl; }
};

void foo() {
    static emitter a("in foo");
    static emitter b("in bar");
    cout << a.str << endl;
    cout << b.str << endl;
}

void bar(const std::function<int(void)>& do_func) { do_func(); }

#define KKK 10
#define lett static emitter

int main(int argc, char*[]) {
    /*
    foo();
    foo();
    foo();
    foo();
    foo();
    foo();
    foo();
    */

    for (int i = 0; i < 10; i++) {
        auto k = []() {
            std::cout << "emitter befor !!" << std::endl;
            static emitter t("helloasdfad;fkasdfja;sdlfkajsdfasdf;lkajdf");
            lett m("asdfasdf");
            std::cout << "emitter after !!" << std::endl;
            std::cout << t.str << "   " << m.str << std::endl;
            foo();
            return 0;
        };
        bar(k);
    }
    for (int j = 1; j < KKK; j++) {
        std::cout << j << std::endl;
    }
}
