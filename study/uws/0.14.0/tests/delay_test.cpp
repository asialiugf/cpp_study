#include <sys/time.h>
#include <uWS/uWS.h>
#include <unistd.h>

#include <chrono>
#include <iostream>

#include "fmtlog-inl.h"
#include "fmtlog.h"
#include "timestamp.h"

#pragma comment(lib, "uWS.lib")

int main() {
    uWS::Hub h;

    struct timeval start;
    struct timeval end;
    // unsigned  long diff;

    std::chrono::high_resolution_clock::time_point t0, t1;

    t0 = std::chrono::high_resolution_clock::now();
    t1 = std::chrono::high_resolution_clock::now();

    auto before = rdtsc();
    auto after  = rdtsc();

    // std::cout << "latency: " << latency << " cycles" << std::endl;

    // --------------------------------------------------------------------------------------------
    // 服务端接收到包后原封不动返回
    h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        // ws->send("--server--");
        std::cout << "Server onConnection send: --server--" << std::endl;
    });

    h.onMessage([ &t0, &t1,&before ](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length,
                             uWS::OpCode opCode) {
        // char tmp[ 256 ];
        // memcpy(tmp, message, length);
        // tmp[ length ] = 0;
        // printf("Server onMessage receive: %s\n", tmp);

        before = rdtsc();
        t0     = std::chrono::high_resolution_clock::now();
        // double span1 = std::chrono::duration_cast<std::chrono::duration<double>>(t1 -
        // t0).count(); printf("server : thedifference is %ld\n", span1);

        ws->send(message, length, opCode);
    });

    bool k = h.listen(3000);
    if (!k) {
        std::cout << " listen error !!" << std::endl;
    }
    // --------------------------------------------------------------------------------------------

    // 客户端连上后发送hello
    h.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        ws->send("--client0000-----------------------------------------------------------------");
        std::cout << "Client onConnection send: --client00--" << std::endl;
    });

    // 客户端打印接收到的消息
    h.onMessage([ &t0, &t1, &before, &after ](uWS::WebSocket<uWS::CLIENT> *ws, char *message,
                                              size_t length, uWS::OpCode opCode) {
        after = rdtsc();
        std::cout << "latency: " << after - before << " cycles" << std::endl;
        t1          = std::chrono::high_resolution_clock::now();
        double span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        fmt::print("benchmark, front latency: {:.1f} ns/msg average\n", span * 1e9);

        char tmp[ 256 ];
        memcpy(tmp, message, length);
        tmp[ length ] = 0;
        printf("Client onMessage receive: %s\n", tmp);
        // usleep(1000);
        ws->send(message, length, opCode);
    });

    h.connect("ws://localhost:3000");
    // --------------------------------------------------------------------------------------------

    h.run();
}
