/*
 * TcpServer_test.cpp
 *
 * @build
 * make libhv && sudo make install
 * g++ -std=c++11 TcpServer_test.cpp -o TcpServer_test -I/usr/local/include/hv -lhv -lpthread
 *
 */

#include <iostream>
#include "TcpServer.h"
#include "TcpClient.h"
#include "htime.h"
#include "timestamp.h"

using namespace hv;

int main(int argc, char* argv[]) {

    auto before = rdtsc();
    auto after = rdtsc();

    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -10;
    }
    int port = atoi(argv[1]);

    TcpServer srv;
    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
        else {
            printf("%s disconnected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };
    srv.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
        channel->write(buf);
        before = rdtsc();
    };
    srv.onWriteComplete = [&](const SocketChannelPtr& channel, Buffer* buf) {
        // before = rdtsc();
        // after = rdtsc();
        // std::cout << "from  server ----  latency: " << after - before << " cycles" << std::endl;
        // printf("> %.*s\n", (int)buf->size(), (char*)buf->data());
        //
        // test
    };
    srv.setThreadNum(1);
    srv.start();

    std::cout << " before dd!!" << std::endl;

    TcpClient cli;
    int connfd = cli.createsocket(port);
    if (connfd < 0) {
        return -20;
    }
    printf("client connect to port %d, connfd=%d ...\n", port, connfd);
    cli.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
            // send(time) every 3s
            setInterval(1000, [channel](TimerID timerID) {
                if (channel->isConnected()) {
                    char str[DATETIME_FMT_BUFLEN] = {0};
                    datetime_t dt = datetime_now();
                    datetime_fmt(&dt, str);
                    channel->write(str);
                }
                else {
                    killTimer(timerID);
                }
            });
        }
        else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };
    cli.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        after = rdtsc() - before;
        std::cout << "from  client latency: " << after << " cycles" << std::endl;
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };

    cli.onWriteComplete = [&before, &after](const SocketChannelPtr& channel, Buffer* buf) {
        // before = rdtsc();
        // after = rdtsc() - before;
        // std::cout << "from  client latency: " << after << " cycles" << std::endl;
        // printf("> %.*s\n", (int)buf->size(), (char*)buf->data());
    };
    // reconnect: 1,2,4,8,10,10,10...
    ReconnectInfo reconn;
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
    cli.start();

    while (1) hv_sleep(10000000);

    //while (1) hv_sleep(1);
    std::cout << " after !!" << std::endl;
    return 0;
}
