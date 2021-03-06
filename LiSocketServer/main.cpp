//
// 本代码是Server端的Demo
// 注意Server端每执行一次程序，都必须在fork()的新进程内完成。
// 因为单个进程只允许1024个socket描述符，如果直接在主程序内使用demo中36～44行的函数，
// 将会导致描述符数量超出范围而使程序无法正常工作，切记。
// 本程序完成一次“指令发送->图片接收->接收结果反馈->图片重新接收”需要进行两次套接字连接，
// 所有函数写在Socketfunc类里，使用时只需要用该类创建对象，
// 构造函数Socketfunc()会完成套接字的创建socket(),端口绑定bind()和监听listen()，
// SocketInit()函数会完成accept()操作，返回新的套接字，
// InstructionSend()函数会执行一次write()，将“Capture...”开头的字符串发送给客户端，
// ImageReceive()函数完成图片的接收，需要指定图片的标号，图片发送完成后，会关闭套接字。
// 之后调用jpgcheck()函数对图片完整性进行分析，若验证图片完整，返回0,若不完整，返回，
// 重新调用初始化函数SocketInit()，完成第二次通信，即图片结果反馈和是否需要重新接收。
// InstructionSend()根据传入参数state的不同，发送成功验证“Verified..”或重传“ReSend..”到客户端。
// ImageReReceive()函数接收重传的图片，需要指定标号和时间，时间用于生成文件名，以覆盖之前不完整的图片，
// 若不需要重传，则直接结束fork()的进程。
// 回调函数请自行声明，安排位置。
// Created by Litthins on 18-5-20.
//

// 测试程序如下，
// 每执行一次通信，必须新fork()一个进程，所有功能在新进程中完成。
//

#include "headers/socketfunc.h"

int main(int argc, char *argv[]) {

    int state_a = 0;//标志变量，用于表示图片验证结果
    int state_b = 0;
    char a[] = "A", b[] = "B", c[] = "C";//标号，用于产生文件名，以区别不同客户端上传的文件
    int porta = 4001;//server端绑定的端口，用于与客户端通信
    int portb = 4002;
    int portc = 4003;

    pid_t pid;
    pid = fork();
    if (pid == 0) {
        Socketfunc Dev1(porta);
        Dev1.SocketInit();
        Dev1.InstructionSend();
        Dev1.ImageReceive(a);
        state_a = Dev1.JpgCheck(Dev1.path);
        Dev1.SocketInit();
        Dev1.InstructionReSend(state_a);
        if (state_a == 1)
            Dev1.ImageReReceive(a, Dev1.calendar);
        sleep(1);
    }

    return 0;
}