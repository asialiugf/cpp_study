#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Base {
    virtual ~Base() = default;  // 虚析构函数 ： 多态基类！
    int test;
    using create_f = std::unique_ptr<Base>();

    static void registrate(std::string const &name, create_f *fp) { registry()[name] = fp; }

    static std::unique_ptr<Base> instantiate(std::string const &name) {
        auto it = registry().find(name);
        return it == registry().end() ? nullptr : (it->second)();
    }

    // static std::default_delete<Base> get_deleter() = default;

    template <typename D>
    struct Registrar {
        explicit Registrar(std::string const &name) { Base::registrate(name, &D::create); }
        // make non-copyable, etc.
    };

   private:
    static std::unordered_map<std::string, create_f *> &registry();
};

std::unordered_map<std::string, Base::create_f *> &Base::registry() {
    static std::unordered_map<std::string, Base::create_f *> impl;
    return impl;
}

struct Derived : Base {
    static std::unique_ptr<Base> create() {
        std::cout << "create !!" << std::endl;
        return std::make_unique<Derived>();
    }  //  创建Derived 返回 Base ！
    Derived *get() { return this; }
    Derived *get_deleter() { return this; }
    int x;
    void set_x(int a) { x = a; }
    // ...
};

template <typename Derived>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &p) {
    return std::unique_ptr<Derived>(p);
}
/*
template <typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del> dynamic_unique_ptr_cast(std::unique_ptr<Base, Del> &&p) {
    if (Derived *result = dynamic_cast<Derived *>(p.get())) {
        p.release();
        return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
    }
    return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
}
*/
namespace {
Base::Registrar<Derived> registrar1("MyClass");
Base::Registrar<Derived> registrar2("MyClass");
Base::Registrar<Derived> registrar3("MyClass");
Base::Registrar<Derived> registrar4("MyClass");
}  // namespace

int main() {
    std::unique_ptr<Base> p = Base::instantiate("MyClass");
    // std::unique_ptr<Derived> x = dynamic_unique_ptr_cast<Derived>(p);
    std::unique_ptr<Derived> x = std::make_unique<Derived>(p);
    Derived *k = x.get();
    k->set_x(100);

    // auto d = dynamic_unique_ptr_cast<Derived, Base>(std::move(b))
    // Derived *x = dynamic_cast<Derived *>(p);
    // x(3);
}
