#include "calcul.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

/*
void raz_msg(int signal) {
	printf("Suppression de la file de message!\n");
	msgctl();
}*/

int msg_id1;
int msg_id2;

int main(int argc, char const *argv[])
{
	int  end = 0;
	struct msg_struct msg;
//	int i_sig;
	/* liberer la zone de messages sur reception de n'importe quel signal */
/*
	for (i_sig = 0 ; i_sig < NSIG ; i_sig++) {
	}*/
	key_t key1; //file des requetes
	key_t key2;
	if((key1=ftok("serveur.c",0))==-1) {
		perror("prb ftok serveur");
		exit(0);
	}
	if((key2=ftok("client.c",0))==-1) {
                perror("prb ftok serveur");
                exit(0);
        }

	if((msg_id1 = msgget(key1,IPC_CREAT | 0660))==-1) {
		perror("prb msget serveur");
		exit(0);
	}
	if((msg_id2 = msgget(key2,IPC_CREAT | 0660))==-1) {
                perror("prb msget serveur");
                exit(0);
        }

	printf("SERVEUR: pret!\n");
	while (end!=1) {
		/* reception */
		if((msgrcv(msg_id1,&msg,sizeof(struct message),1L,0))==-1) {
			perror("prb recieve serveur");
			exit(0);
		}
		if(strcmp(msg.message.buffeur,"@")==0){
			end = 1;
			strcpy(msg.message.buffeur,"fin de transmission");
			printf("fin de transmission\n");
		}
		else {
			printf("SERVEUR: reception d'une requete de la part de: %d\n",msg.message.pid);
			for(int i = 0;i<strlen(msg.message.buffeur);i++){
				if (islower(msg.message.buffeur[i])){
				msg.message.buffeur[i] = toupper(msg.message.buffeur[i]);
				}
			}
		}
			msg.type = msg.message.pid;
			if(msgsnd(msg_id2,&msg,sizeof(struct message),0)==-1) {
				perror("prb send client");
				exit(0);
			}
	}
	return EXIT_SUCCESS;
}

