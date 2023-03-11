#include <iostream>
#include <cassert>

class Singleton {
public:
    // static Singleton instance;
    ~Singleton() {
        std::cout << "destructor called!" << std::endl;
    }
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    static void INIT(const std::string &a) noexcept {
        static Singleton instance(a);
        // static App ap{b};
        instance_ = &instance;
    }
    static Singleton &get_instance() {
        // static Singleton instance;
        // instance.Filename_ = a;
        assert(instance_);
        return *instance_;
    }

    void show() {
        std::cout << this->Filename_ << std::endl;
    }

private:
    Singleton(const std::string a) {
        Filename_ = a;
        std::cout << "constructor called!" << std::endl;
    }
    std::string Filename_;
    static Singleton *instance_;
};

int main(int argc, char *argv[]) {
    Singleton::INIT("ad;fkljad");
    Singleton &instance_1 = Singleton::get_instance();
    Singleton &instance_2 = Singleton::get_instance();
    instance_1.show();
    instance_2.show();
    std::cout << "ahah" << std::endl;
    return 0;
}
