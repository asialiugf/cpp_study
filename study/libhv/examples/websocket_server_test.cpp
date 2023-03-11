/*
 * websocket server
 *
 * @build   make examples
 * @server  bin/websocket_server_test 9999
 * @client  bin/websocket_client_test ws://127.0.0.1:9999/
 * @js      html/websocket_client.html
 *
 */
#include <iostream>
#include "WebSocketServer.h"
#include "WebSocketClient.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"
#include "timestamp.h"

/*
 * #define TEST_WSS 1
 *
 * @build   ./configure --with-openssl && make clean && make
 *
 * @server  bin/websocket_server_test 9999
 *
 * @client  bin/websocket_client_test ws://127.0.0.1:9999/
 *          bin/websocket_client_test wss://127.0.0.1:10000/
 *
 */
#define TEST_WSS 0

using namespace hv;

class MyContext {
public:
    MyContext() { timerID = INVALID_TIMER_ID; }
    ~MyContext() {}

    int handleMessage(const std::string& msg) {
        printf("onmessage: %s\n", msg.c_str());
        return msg.size();
    }

    TimerID timerID;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -10;
    }
    int port = 9999;

    WebSocketService ws;
    ws.onopen = [](const WebSocketChannelPtr& channel, const std::string& url) {
        printf("onopen: GET %s\n", url.c_str());
        MyContext* ctx = channel->newContext<MyContext>();
        // send(time) every 1s
        ctx->timerID = setInterval(1000, [channel](TimerID id) {
            char str[DATETIME_FMT_BUFLEN] = {0};
            datetime_t dt = datetime_now();
            datetime_fmt(&dt, str);
            channel->send(str);
        });
    };
    ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg) {
        MyContext* ctx = channel->getContext<MyContext>();
        ctx->handleMessage(msg);
    };
    ws.onclose = [](const WebSocketChannelPtr& channel) {
        printf("onclose\n");
        MyContext* ctx = channel->getContext<MyContext>();
        if (ctx->timerID != INVALID_TIMER_ID) {
            killTimer(ctx->timerID);
        }
        channel->deleteContext<MyContext>();
    };

    websocket_server_t server;
    server.port = port;
#if TEST_WSS
    server.https_port = port + 1;
    hssl_ctx_init_param_t param;
    memset(&param, 0, sizeof(param));
    param.crt_file = "cert/server.crt";
    param.key_file = "cert/server.key";
    param.endpoint = HSSL_SERVER;
    if (hssl_ctx_init(&param) == NULL) {
        fprintf(stderr, "hssl_ctx_init failed!\n");
        return -20;
    }
#endif
    server.ws = &ws;

    std::cout << "before server !!" << std::endl;
    websocket_server_run(&server);

    std::cout << "after server !!" << std::endl;

    const char* url = "ws://127.0.0.1:9999/";

    WebSocketClient wsc;
    wsc.onopen = [&wsc]() {
        printf("onopen\n");
        wsc.send("hello");
    };
    wsc.onclose = []() { printf("onclose\n"); };
    wsc.onmessage = [](const std::string& msg) { printf("onmessage: %s\n", msg.c_str()); };

    // reconnect: 1,2,4,8,10,10,10...
    ReconnectInfo reconn;
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    wsc.setReconnect(&reconn);
    wsc.open(url);

    return 0;
}
