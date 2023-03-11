// Config.hpp
#include <iostream>
#include <cassert>

struct Config {
public:
    Config(const Config &) = delete;
    Config &operator=(const Config &) = delete;
    static void init(std::string const file_name) noexcept(true);

    static Config &GetInstance() noexcept {
        assert(intstance_);
        return *intstance_;
    }
    void show() {
        std::cout << this->file_name_ << std::endl;
    }

private:
    ~Config() {
        std::cout << "destructor called!" << std::endl;
    }
    Config(std::string const file_name) :
        file_name_{file_name} {
        std::cout << "constructor called!" << std::endl;
    }

    std::string file_name_;
    static Config *intstance_;
};

void Config::init(std::string const file_name) noexcept {
    static Config app{file_name};
    intstance_ = &app;
}

// Config.cpp

Config *Config::intstance_ = nullptr;

// main.cpp

int main() {
    Config::init("truqqqe");
    auto &ap = Config::GetInstance();
    ap.show();
    std::cout << " hello singleton c++ " << std::endl;
    Config::init("truquuausfdqqe");
    auto &ap1 = Config::GetInstance();
    ap1.show();
}