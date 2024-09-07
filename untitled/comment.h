#ifndef COMMENT_H
#define COMMENT_H
#include <stdio.h>
#include <string.h>

struct QMSG//传输消息的船
{
    short type;//传输消息的类型，通过对类型的判断完成相应的方法
    char data[1022];
};
struct register_node//注册消息类
{
    register_node(const char * name,const char *password)
    {
        strcpy(this->name,name);
        strcpy(this->password,password);
    }
    char name[128];
    char password[128];
};

#endif // COMMENT_H
