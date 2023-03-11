#include <uWS/WebSocketProtocol.h>
#include <uWS/uWS.h>

#include <algorithm>
#include <iostream>
#include <thread>

#define random(x) (rand() % x)

struct Tick {
    int    n;
    double h;
    double o;
    double c;
    double l;
    double v;
    char   name[ 100 ];
};

size_t len = sizeof(Tick);

int main() {
    std::vector<std::thread *> threads(std::thread::hardware_concurrency());
    std::transform(threads.begin(), threads.end(), threads.begin(), [](std::thread *t) {
        return new std::thread([]() {
            Tick *tt = new Tick[ 1 ];

            uWS::Hub h;

            h.onMessage([ & ](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length,
                              uWS::OpCode opCode) {
                std::cout << "on Message  ok !!" << std::endl;
                std::cout << "len:: " << len << std::endl;
                for (int n = 0; n < 1000; n++) {
                    //   ws->send(message, length, opCode);
                    //  usleep(10000);
                }
                for (int n = 0; n < 10000000; n++) {
                    // while (true) {
                    tt[ 0 ].h = (double)random(1000);
                    tt[ 0 ].o = (double)random(1000);
                    tt[ 0 ].c = (double)random(1000);
                    tt[ 0 ].l = (double)random(1000);
                    tt[ 0 ].n = n;
                    // ws->send(std::string_view((char *)tt, len), uWS::OpCode::BINARY);
                    // std::cout << "send ok !!" << std::endl;

                    ws->send((char *)&tt[ 0 ], len, uWS::OpCode::BINARY);
                    // usleep(1000000);
                    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    // std::cout << "send :::  " << n << std::endl;
                }
                // ws->send(message, length, opCode);
                std::cout << "on Message  ok ------end  !!" << std::endl;
            });

            // This makes use of the SO_REUSEPORT of the Linux kernel
            // Other solutions include listening to one port per thread
            // with or without some kind of proxy inbetween
            if (!h.listen(9001, nullptr, uS::ListenOptions::REUSE_PORT)) {
                std::cout << "Failed to listen" << std::endl;
            }
            h.run();
        });
    });

    std::for_each(threads.begin(), threads.end(), [](std::thread *t) { t->join(); });
}
