#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#define SHM_SIZE 1024
int main(){
	key_t key = ftok("shmfile", 65);
	int shmid=shmget(key, SHM_SIZE, IPC_CREAT | 0666);
	if (shmid==-1){
		perror("shmger");
		exit(1);
	}
	void *shm = shmat(shmid, NULL, 0);
	if (shm==(void *)-1){
		perror ("shmat");
		exit(1);
	}
	if (fork()==0){
		char *message="Привет из дочернего процесса!";
		memcpy(shm, message, strlen(message) +1);
		shmdt(shm);
		exit(0);
	} else{
		sleep(1);
		char *message = (char *)shm;
		printf("Сообщение из дочернего процесса: %s\n", message);
		shmdt(shm);
		shmctl(shmid,IPC_RMID,NULL);
	}
	return 0;
}

