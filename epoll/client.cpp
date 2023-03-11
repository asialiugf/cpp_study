// client.cpp
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

// server port
#define SERVER_PORT 8888

// epoll 支持的最大并发量
#define EPOLL_SIZE 5000

// message buffer size
#define BUF_SIZE 0xFFFF

// exit
#define EXIT "EXIT"

// 设置sockfd，pipefd非阻塞
int setnonblocking(int sockfd) {
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
    return 0;
}

int addfd(int epollfd, int fd, bool enable_et) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;  // 输入出发epoll-event
    if (enable_et) {
        ev.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
}

int main(int argc, char* argv[]) {
    // 用户连接的服务器IP、端口
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    const char* servInetAddr = "127.0.0.1";
    inet_pton(AF_INET, servInetAddr, &serverAddr.sin_addr);

    // 创建socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("sock error");
        exit(-1);
    }

    // 连接服务端
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    // 创建管道，其中fd[0]用于父进程读，fd[1]用于子进程写
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("pipe error");
        exit(-1);
    }

    // 1 创建epoll
    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }
    static struct epoll_event events[2];
    // 将sock和管道读端都加到内核事件表中
    addfd(epfd, sock, true);
    addfd(epfd, pipe_fd[0], true);

    // 表示客户端是否正常工作
    bool isClientwork = true;

    // 聊天信息缓冲区
    char message[BUF_SIZE];

    // Fork
    int pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) {  // 子进程
        // 子进程负责写入管道，因此先关闭读端
        close(pipe_fd[0]);
        printf("Please input 'exit' to exit the chat room\n");

        while (isClientwork) {
            bzero(&message, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            // 客户端输出exit，退出
            if (strncasecmp(message, EXIT, strlen(EXIT)) == 0) {
                isClientwork = 0;
            } else {
                // 子进程将信息写入管道
                if (write(pipe_fd[1], message, strlen(message) - 1) < 0) {
                    {
                        perror("fork error");
                        exit(-1);
                    }
                }
            }
        }
    } else {  // pid > 0 父进程
        // 父进程负责读管道数据，因此先关闭写端
        close(pipe_fd[1]);

        while (isClientwork) {
            int epoll_events_count = epoll_wait(epfd, events, 2, -1);
            // 处理就绪事件
            for (int i = 0; i < epoll_events_count; i++) {
                bzero(&message, BUF_SIZE);
                // 服务端发来消息
                if (events[i].data.fd == sock) {
                    // 接受服务端消息
                    int ret = recv(sock, message, BUF_SIZE, 0);

                    // ret = 0  服务端关闭
                    if (ret == 0) {
                        printf("Server closed connection: %d\n", sock);
                        close(sock);
                        isClientwork = 0;
                    } else {
                        printf("%s\n", message);
                    }
                } else {
                    // 子进程写入事件发生，父进程处理并发送数据
                    int ret = read(events[i].data.fd, message, BUF_SIZE);
                    if (ret = 0) {
                        isClientwork = 0;
                    } else {
                        send(sock, message, BUF_SIZE, 0);
                    }
                }
            }
        }
    }

    if (pid) {
        close(pipe_fd[1]);
        close(sock);
    } else {
        close(pipe_fd[0]);
    }

    return 0;
}
