#ifndef __CALCUL_H__
#define __CALCUL_H__
#include <stdlib.h>
#include <sys/types.h>


struct message {
	pid_t pid;
	char buffeur[20];
};

struct msg_struct {
    long type;
    struct message message;
};

#endif /* __CALCUL_H__ */
