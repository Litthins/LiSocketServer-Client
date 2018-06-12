//
// 本文件为Socketfunc类的定义文件，详细参考main.cpp文件注释
// Created by Litthins on 18-5-20.
//

#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include "../headers/socketfunc.h"

Socketfunc::Socketfunc() {}//构造函数为空

//初始化函数
int Socketfunc::SocketInit(int port, char *ip) {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket created error!");
        exit(1);
    } else {
        printf("socket created successfully!\n");
        printf("socket id:%d\n", sockfd);
    }
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    while (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) < 0) {
        //perror("connect error!");
    }
    printf("connected successfully!\n");
    printf("remote ip:%s\n", ip);
    printf("remote port:%d\n", port);
    return 0;
}

//指令接收函数
int Socketfunc::InstructionReceive() {
    while (read(sockfd, buf, sizeof(buf)) == -1) {
        printf("Read instruction error!");
    }
    if (buf[0] == 'C') {//字符串“Capture”开头
        printf("Received:%s\n", buf);
        return 0;
    }
    if (buf[0] == 'R') {//字符串“ReSend”开头
        printf("Received:%s\n", buf);
        return 1;
    }
    if (buf[0] == 'V') {//字符串“Verified”开头
        printf("Received:%s\n", buf);
        return 0;
    }
}

//图片发送函数
int Socketfunc::ImageSend(char *path) {
    if ((fp = fopen(path, "rb")) == NULL) {
        perror("open file error!");
        exit(1);
    }
    bzero(buf, sizeof(buf));
    while (!feof(fp)) {
        printf("%d", feof(fp));
        len = static_cast<int>(fread(buf, 1, sizeof(buf), fp));
        printf("fread OK.len=%d", len);
        write(sockfd, buf, len);
        printf("write OK.");
        printf("%d\n", feof(fp));
    }
    fclose(fp);
    close(sockfd);
    close(sockfd);
    close(sockfd);
    close(sockfd);
    memset(buf,0, sizeof(buf));
    printf("Send Completed.\n");
    return 0;
}

//关闭Socket函数，保留用
int Socketfunc::Closesock() {
    close(sockfd);
    return 0;
}