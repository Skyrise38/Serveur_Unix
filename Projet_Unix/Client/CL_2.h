#include "CL_def.h"

typedef int SEMAPHORE;

char* connection_msg(int msqid);
void deconection_msg(int msqid);
key_t Creer_cle(char *nom_fichier);
int CreationMessagerie();
BUF* connection_shm(char* cle_shm);
void detachement_shm(BUF* ptr_shm);


void my_handler1(int n);
void my_handler2(int n);



SEMAPHORE Creer_sem(key_t key);
void Detruire_sem(SEMAPHORE sem);
void Changer_sem(SEMAPHORE sem, int val);
void P(SEMAPHORE sem);
void V(SEMAPHORE sem);


int CreationMutex(void);
int DestructionMutex(int);
int Init_Mutex(int, int);
int Ps(int, int);
int Vs(int, int);





