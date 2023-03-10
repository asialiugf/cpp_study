#include <iostream>
#include <resumable>
using namespace std;
struct resumable_thing {
    struct promise_type {
        resumable_thing get_return_object() {
            return resumable_thing(coroutine_handle<promise_type>::from_promise(*this));
        }
        auto initial_suspend() {
            return suspend_never{};
        }
        auto final_suspend() {
            return suspend_never{};
        }
        void return_void() {
        }
    };
    coroutine_handle<promise_type> _coroutine = nullptr;
    resumable_thing() = default;
    resumable_thing(resumable_thing const &) = delete;
    resumable_thing &operator=(resumable_thing const &) = delete;
    resumable_thing(resumable_thing &&other) :
        _coroutine(other._coroutine) {
        other._coroutine = nullptr;
    }
    resumable_thing &operator=(resumable_thing &&other) {
        if (&other != this) {
            _coroutine = other._coroutine;
            other._coroutine = nullptr;
        }
    }
    explicit resumable_thing(coroutine_handle<promise_type> coroutine) :
        _coroutine(coroutine) {
    }
    ~resumable_thing() {
        if (_coroutine) { _coroutine.destroy(); }
    }
    void resume() {
        _coroutine.resume();
    }
};
resumable_thing counter() {
    cout << "counter: called\n";
    for (unsigned i = 1;; i++) {
        co_await std::suspend_always{};
        cout << "counter:: resumed (#" << i << ")\n";
    }
}
int main() {
    cout << "main:    calling counter\n";
    resumable_thing the_counter = counter();
    cout << "main:    resuming counter\n";
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    cout << "main:    done\n";
    return 0;
}
// 上面代码的输出如下
//: main
//: calling countercounter
//: calledmain
//: resuming countercounter::resumed(#1)
// counter::resumed(#2)
// counter::resumed(#3) counter::resumed(#4) counter::resumed(#5) main : done
