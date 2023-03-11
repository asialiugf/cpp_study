// server.cpp
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <list>

using namespace std;

// 选用list存放sockfd
list<int> clients_list;

// server port
#define SERVER_PORT 8888

// epoll支持的最大并发量
#define EPOLL_SIZE 5000

// message buf size
#define BUF_SIZE 0xFFFF

#define SERVER_WELCOME \
    "Welcome you join  to the chat room! Your chat ID is: Client #%d"

#define SERVER_MESSAGE "ClientID %d say >> %s"

// exit
#define EXIT "EXIT"

#define CAUTION "There is only one int the char room!"

int setnonblocking(int sockfd) {
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
    return 0;
}

void addfd(int epollfd, int fd, bool enable_et) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (enable_et) {
        ev.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
}

int sendBroadcastmessage(int clientfd) {
    char buf[BUF_SIZE], message[BUF_SIZE];
    // 清零
    bzero(buf, BUF_SIZE);
    bzero(message, BUF_SIZE);

    printf("read from client(clientID = %d)\n", clientfd);
    int len = recv(clientfd, buf, BUF_SIZE, 0);
    // len=0 client关闭了连接
    if (len == 0) {
        close(clientfd);
        clients_list.remove(clientfd);
        printf(
            "ClientID = %d closed.\n now there are %d client in the char "
            "room\n",
            clientfd, (int)clients_list.size());
    } else {  // 进行广播
        if (clients_list.size() == 1) {
            send(clientfd, CAUTION, strlen(CAUTION), 0);
            return len;
        }
        sprintf(message, SERVER_MESSAGE, clientfd, buf);

        list<int>::iterator iter;
        for (iter = clients_list.begin(); iter != clients_list.end(); iter++) {
            if (*iter != clientfd) {
                if (send(*iter, message, BUF_SIZE, 0) < 0) {
                    perror("error");
                    exit(-1);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // 服务器IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 创建监听socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("listenfd");
        exit(-1);
    }
    printf("listen socket created");

    // 绑定地址
    if (bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }

    // 监听
    int ret = listen(listenfd, 5);
    if (ret < 0) {
        perror("listen error");
        exit(-1);
    }

    // 在内核中创建事件表
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }
    printf("epoll created, epoll size = %d\n", epfd);

    static struct epoll_event events[EPOLL_SIZE];

    // 往内核事件表里添加事件
    addfd(epfd, listenfd, true);

    // 主循环
    while (1) {
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (epoll_events_count < 0) {
            perror("epoll failure");
            break;
        }
        printf("epoll event counts = %d\n", epoll_events_count);

        for (int i = 0; i < epoll_events_count; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof(struct sockaddr_in);
                int clientfd =
                    accept(listenfd, (struct sockaddr*)&client_address,
                           &client_addrLength);

                addfd(epfd, clientfd, true);

                // 服务端用list保存用户连接
                clients_list.push_back(clientfd);

                std::cout << clients_list.size() << std::endl;

                // 服务端发送欢迎消息
                /*
                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientfd);
                int ret = send(clientfd, message, BUF_SIZE, 0);
                if (ret < 0) {
                    perror("error000");
                    exit(-1);
                }
                */
            } else {
                int ret = sendBroadcastmessage(sockfd);
                if (ret < 0) {
                    perror("error111");
                    exit(-1);
                }
            }
        }
    }

    close(listenfd);  // 关闭socket
    close(epfd);      // 关闭内核
    return 0;
}