#ifndef __CALCUL_H__
#define __CALCUL_H__

#include <unistd.h>
#include <sys/types.h>

struct msg_struct {
    long type;
    pid_t pid;
    char operation;
    int ope1;
    int ope2;
};

struct infos{
    pid_t pid;
    char operation;
    int ope1;
    int ope2;
};

#endif /* __CALCUL_H__ */
