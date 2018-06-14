//
// 本文件是socketfunc类函数的定义文件，详细使用方法请参考main.cpp文件
// Created by raspberry on 18-5-20.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "../headers/socketserverfunc.h"

#define BUFSIZE 2048

//构造函数
Socketserverfunc::Socketserverfunc(int port) {
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket created error!");
        exit(1);
    } else {
        printf("socket created successfully!\n");
        printf("socket id:%d\n", sockfd);
    }
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<uint16_t>(port));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &bReuseaddr, sizeof(bool));
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(struct linger));
    if (bind(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) < 0) {
        perror("bind error!");
        exit(1);
    } else {
        printf("bind port successfully!\n");
        printf("local port:%d\n", port);
    }
    if (listen(sockfd, 6) < 0) {
        perror("listen error!");
    } else {
        printf("listening......\n");
    }
}

//初始化函数
int Socketserverfunc::SocketInit() {
    sin_size = sizeof(struct sockaddr_in);
    if ((newsockfd = accept(sockfd, (struct sockaddr *) (&client_addr), reinterpret_cast<socklen_t *>(&sin_size))) <
        0) {
        perror("accept error!");
        exit(1);
    }
    printf("accepted a new connection.\n");
    printf("new socket id:%d\n", newsockfd);
    printf("connect from %s\n", inet_ntoa(client_addr.sin_addr));
    return 0;
}

//指令发送函数
int Socketserverfunc::InstructionSend(char *signal) {
    printf("This is Instruction Sender.\n");
    calendar = time(NULL);
    //strcpy(msgbuf, "Capture --> ");
    strcpy(msgbuf, signal);
    strcat(msgbuf, ctime(&calendar));
    if (write(newsockfd, msgbuf, strlen(msgbuf)) == -1) {
        printf("write error!");
        exit(1);
    }
    return 0;
}

//指令重发函数
int Socketserverfunc::InstructionReSend(int state) {
    printf("This is Instruction Re Sender.\n");
    if (state == 1)
        strcpy(msgbuf, "Resend ----> ");
    if (state == 0)
        strcpy(msgbuf, "Verified --> ");
    strcat(msgbuf, ctime(&calendar));
    printf("%s\n",msgbuf);
    if (write(newsockfd, msgbuf, strlen(msgbuf)) == -1) {
        printf("write error!");
        exit(1);
    }
    return 0;
}

//图片接收函数
int Socketserverfunc::ImageReceive(char *id) {
    printf("This is Image Receiver.\n");
    strcat(path, strcat(strcat(strncat(temp, ctime(&calendar), strlen(ctime(&calendar)) - 1), id), ".jpg"));
    printf("image path is: '%s'\n", path);
    if ((fp = fopen(path, "wb+")) == NULL) {
        perror("create image file error!");
        exit(1);
    }
    int rn;
    printf("ImageReceive --> ");
    printf("%s\n", path);
    while (true) {
        printf("read Start.");
        rn = static_cast<int>(read(newsockfd, msgbuf, sizeof(msgbuf)));
        printf("read OK.rn=%d",rn);
        if (rn == 0)
            break;
        fwrite(msgbuf, 1, static_cast<size_t>(rn), fp);
        printf("fwrite OK.\n");
    }
    printf("Receive Completed.\n");
    fclose(fp);
    close(newsockfd);
    printf("newsockfd closed.\n");
    return 0;
}

//图片重新接收函数
int Socketserverfunc::ImageReReceive(char *id, time_t calendar) {
    strcat(path, strcat(strcat(strncat(temp, ctime(&calendar), strlen(ctime(&calendar)) - 1), id), ".jpg"));
    printf("image path is: '%s'\n", path);
    if ((fp = fopen(path, "wb")) == NULL) {
        perror("create image file error!");
        close(sockfd);
        exit(1);
    }
    int rn;
    printf("ImageReReceive --> ");
    printf("%s", strcat(ctime(&calendar), "\n"));
    while (true) {
        printf("read Start.");
        rn = static_cast<int>(read(newsockfd, msgbuf, sizeof(msgbuf)));
        printf("read OK.rn=%d",rn);
        if (rn == 0)
            break;
        fwrite(msgbuf, 1, rn, fp);
        printf("fwrite OK.\n");
    }
    printf("Re Receive Completed.\n");
    fclose(fp);
    memset(path,0, sizeof(path));
    memset(temp,0, sizeof(temp));
    close(newsockfd);
    return 0;
}

//socket关闭函数，保留
int Socketserverfunc::Closesock() {
    close(sockfd);
    close(newsockfd);
    return 0;
}