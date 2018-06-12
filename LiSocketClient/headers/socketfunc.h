// 本文件为Socketfunc类的声明，详细定义在socketfunc.cpp文件中
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

class Socketfunc{

public:
    int sockfd;//套接子描述符
    struct sockaddr_in server_addr;
    char buf[2048] = "\0";//缓冲buf
    FILE *fp;//打开图片时使用的指针
    int len;

    Socketfunc();

    int SocketInit(int port, char *ip);

    int InstructionReceive();

    int ImageSend(char *path);

    int Closesock();

};