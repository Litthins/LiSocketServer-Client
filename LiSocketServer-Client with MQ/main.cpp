#include <pthread.h>
#include "headers/msgqfunc.h"
#include "headers/socketclientfunc.h"
#include "headers/socketserverfunc.h"

void * SocketServer(void * arg);
void * SocketClient(void * arg);
void * Camera(void * arg);
void * Tensor(void * arg);

typedef struct msgq_struct {
    char *msgbuf;
    Msgq *MS;
} ARG;

int main() {

    char msgbuf[] = "/home/raspberry/CLionProjects/CsocketServer/cmake-build-debug";//存放于消息队列中的图片地址
    Msgq Ms;
    ARG msgqarg;//用于在线程中传递参数的结构体
    msgqarg.msgbuf = msgbuf;//图片字符串的地址
    msgqarg.MS = &Ms;//消息队列对象的地址
    pthread_t tid_socket,tid_camera,tid_tensor;
    //pthread_create(&tid_socket, nullptr,SocketServer, nullptr);//在Server端中使用
    //pthread_create(&tid_socket, nullptr,SocketClient, nullptr);//在client端使用
    pthread_create(&tid_camera, nullptr,Camera, (void *) &msgqarg);//可根据需要传入参数，将最后一个nullptr用指针替换即可
                                                        //Camera中可以将图片路径加入消息队列
    //sleep(0.5);
    pthread_create(&tid_tensor, nullptr,Tensor, (void *) &msgqarg);//可根据需要传入参数，将最后一个nullptr用指针替换即可
                                                        //Tensor中可以将图片路径取出消息队列
    //sleep(0.5);

    return 0;
}
void * Camera(void * arg){
    //此处安排摄像头函数
    //此处安排消息队列处理函数，将图片路径加入消息队列
    ARG *msgqarg = (ARG *) arg;
    (*(msgqarg->MS)).Msgqa(msgqarg->msgbuf);//函数Msgqa(char *msg),向消息队列中写图片地址
    return 0;
}

void * Tensor(void * arg){
    //此处安排消息队列处理函数，将图片路径取出消息队列
    ARG *msgqarg = (ARG *) arg;
    (*(msgqarg->MS)).Msgqb();//函数Msgqb(),从消息队列中读图片地址
    //此处安排图像处理函数
    return 0;
}

void * SocketServer(void * arg){

    int porta = 4001;//server端绑定的端口，用于与客户端通信
    char signal[] = "RX ";

    Socketserverfunc Dev1(porta);
    Dev1.SocketInit();
    Dev1.InstructionSend(signal);
    Dev1.Closesock();
    sleep(1);

    return 0;
}

void * SocketClient(void * arg) {

    int porta = 4001;//服务器通讯端口
    char ip[] = "192.168.43.79";//远端服务器ip

    Socketclientfunc Dev1;
    Dev1.SocketInit(porta, ip);
    Dev1.InstructionReceive();
    Dev1.Closesock();
    sleep(1);

    return 0;
}