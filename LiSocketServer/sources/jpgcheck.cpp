//
// 本文件是socketfunc类中jpgcheck()函数的定义，使用方法见main.cpp文件
// 注意！本函数通过检查图片文件的末尾是否符合jpg文件标准末尾来检验图片的完整性，
// 但服务端接收到的文件是客户端的一个拷贝，末尾符合要求并不能保证拷贝文件与源文件的一致性。
// 如对图片校验有严格要求，建议使用其他方法。
// 若校验成功，则返回0,否则，返回1.
// Created by Litthins on 18-5-20.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "../headers/socketfunc.h"


int Socketfunc::JpgCheck(char *path) {
    printf("This is jpg checker.\n");
    FILE *fp, *ft;
    int mchar;
    char buf[2] = "\0";
    if ((fp = fopen(path, "rb")) == NULL) {
        perror("File open");
        exit(1);
    }
    while (!feof(fp)) {
        fread(buf, 1, 2, fp);
    }
    ft = fopen("./check.txt", "wb");
    fwrite(buf, 1, 2, ft);
    fclose(ft);
    fclose(fp);
    ft = fopen("./check.txt", "r");
    mchar = fgetc(ft);
    printf("Verify Char:%d\n", mchar);
    if (mchar != 255) {
        fclose(ft);
        return 1;
    }
    if ((fp = fopen(path, "rb")) == NULL) {
        perror("File open");
        exit(1);
    }
    while (!feof(fp)) {
        fread(buf, 1, 1, fp);
    }
    ft = fopen("./check.txt", "wb");
    fwrite(buf, 1, 1, ft);
    fclose(ft);
    fclose(fp);
    ft = fopen("./check.txt", "r");
    mchar = fgetc(ft);
    printf("Verify Char:%d\n", mchar);
    if (mchar != 217) {
        fclose(ft);
        return 1;
    }
    memset(path,0, sizeof(path));
    memset(temp,0, sizeof(temp));
    return 0;
}