//
// 本代码是Client端的Demo
// 注意Client端每执行一次程序，都必须在fork()的新进程内完成。
// 因为单个进程只允许1024个socket描述符，如果直接在主程序内使用demo中24～30行的函数，
// 将会导致描述符数量超出范围而使程序无法正常工作，切记。
// 本程序完成一次“指令接收->图片发送->成功接收确认->图片重传”需要进行两次套接字连接，
// 所有函数写在Socketfunc类里，使用时只需要用该类创建对象，
// SocketInit()函数会完成套接字的创建socket()和连接connect()，需要指定端口和ip，
// InstructionReceive()函数会执行一次read()，接收由服务端发来的指令，根据指令的不同有不同返回值，
// ImageSend()函数完成图片的发送，需要指定图片的路径，图片发送完成后，会关闭套接字。
// 之后重新调用初始化函数SocketInit()，完成第二次通信，即图片接受确认和是否需要重传。
// 若ImageReceive()函数返回1,则需要重传，否则直接结束fork()的进程。
// 回调函数请自行声明，安排位置。
// Created by Litthins on 18-5-20.
//

// 测试程序如下，
// 每执行一次通信，必须新fork()一个进程，所有功能在新进程中完成。

#include "headers/socketfunc.h"

int main(int argc, char *argv[]) {
    int porta = 4001;//服务器通讯端口
    char ip[] = "192.168.43.79";//远端服务器ip
    char path_a[] = "/home/uesrname/Downloads/test.jpg";//需要上传的图片路径


    pid_t pid;

    pid = fork();
    if (pid == 0) {
        Socketfunc Dev1;
        Dev1.SocketInit(porta, ip);
        Dev1.InstructionReceive();
        Dev1.ImageSend(path_a);
        Dev1.SocketInit(porta, ip);
        if (1 == Dev1.InstructionReceive())
            Dev1.ImageSend(path_a);
        sleep(1);
    }
    return 0;
}