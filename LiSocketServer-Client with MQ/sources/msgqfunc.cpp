//
// Created by raspberry on 18-6-4.
//

#include "msgqfunc.h"

Msgq::Msgq() {
    pid = getpid();
    if ((qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
}

int Msgq::Msgqa(char *msg) {
    struct msg msg_a;
    msg_a.msg_types = pid;
    memset(msg_a.msg_buf, 0, sizeof(msg_a.msg_buf));
    strcat(msg_a.msg_buf, msg);
    if ((msgsnd(qid, &msg_a, sizeof(msg_a.msg_buf), 0)) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("Successfully send the message -> queue: %d,pid: %d \n", qid, pid);
    memset(msg_a.msg_buf, 0, sizeof(msg_a.msg_buf));
    return 0;
}

int Msgq::Msgqb() {
    int len;
    struct msg msg_b;
    if ((len = static_cast<int>(msgrcv(qid, &msg_b, sizeof(msg_b.msg_buf), 0, 0))) > 0) {
        //msg_b.msg_buf[len] = '\0';
        printf("Read queue,id: %d\n", qid);
        printf("Message type: %ld\n", msg_b.msg_types);
        printf("Message length: %d bytes\n", len);
        printf("Message text %s\n", msg_b.msg_buf);
    } else if (len == 0)
        printf("NO message in queue: %d\n", qid);
    else {
        perror("msgrcv");
        exit(1);
    }
    return 0;
}