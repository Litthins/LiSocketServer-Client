//
// 本文件是Socketfunc类的声明文件，详细定义请参照socketfunc.cpp文件
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

#define BUFSIZE 2048

class Socketserverfunc {

public:
    int sockfd, newsockfd;//套接字描述符
    int sin_size;
    bool bReuseaddr = true;//用于设置地址复用
    time_t calendar = 0;//用于存放时间变量
    struct linger so_linger;//用于设置关闭socket的方式
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(struct sockaddr_in);
    char msgbuf[BUFSIZE];//缓冲buf
    FILE *fp;//操作图片文件的指针
    char temp[64] = "\0";//用于生成图片文件名和地址
    char path[64] = "\0";//用于生成图片文件名和地址

    Socketserverfunc(int port);

    int SocketInit();

    int InstructionSend(char *signal);

    int InstructionReSend(int state);

    int ImageReceive(char *id);

    int ImageReReceive(char *id, time_t calendar);

    int JpgCheck(char *path);

    int Closesock();
};

