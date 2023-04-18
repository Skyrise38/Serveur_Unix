typedef int SEMAPHORE;
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
