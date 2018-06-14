//
// Created by raspberry on 18-6-4.
//

#include <sys/msg.h>
#include <zconf.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define MSGBUFSIZE 128

struct msg {
    long int msg_types;
    char msg_buf[MSGBUFSIZE];
};

class Msgq {

private:

    int pid;
    int qid;

public:

    Msgq();

    int Msgqa(char *msg);

    int Msgqb();

};