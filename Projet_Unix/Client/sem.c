
#include "sem.h"

SEMAPHORE Creer_sem(key_t key)
{
  SEMAPHORE sem;
  int r;
  int nombre_de_sem=1;
  int val_init=0;
  sem = semget(key, nombre_de_sem,IPC_CREAT|IPC_EXCL|0666);
  if (sem < 0)
    {
      perror("Creer_sem : semget");
      exit(EXIT_FAILURE);
    }
  r = semctl(sem,nombre_de_sem-1,SETVAL,val_init);
  if (r <0)
    {
      perror("Creer_sem : semctl");
      exit(EXIT_FAILURE);
    }
  return sem;
}

void Detruire_sem(SEMAPHORE sem)
{
    int nombre_de_sem=1;
    if (semctl(sem,nombre_de_sem-1,IPC_RMID,0) != 0)
      {
	perror("Detruire_sem");
	exit(EXIT_FAILURE);
      }

}
void Changer_sem(SEMAPHORE sem, int val)
{
  struct sembuf sb[1];
  int nombre_de_sem=1;

  sb[0].sem_num = nombre_de_sem-1;
  sb[0].sem_op = val;
  sb[0].sem_flg=0;

  if (semop(sem,sb,nombre_de_sem) != 0)
    {
	perror("Changer_sem");
	exit(EXIT_FAILURE);
    }
}

void P(SEMAPHORE sem)
{
  Changer_sem(sem,-1);
}

/* **************************************** */
/* **************************************** */
void V(SEMAPHORE sem)
{
  Changer_sem(sem,1);
}


int CreationMutex()
{
  key_t key;
  int  semid;
  if (( key = ftok(CleServeur,'S')) < 0 )
    return CLEerr;
  semid = semget(key, NVOIES, 0666);
  return semid;
}


/*********************	DESTRUCTION DU MUTEX
 *  ENTREE: semid = identificateur de la famille de semaphores
 *  SORTIE: neant
 *  RETOUR: code erreur -1  ou 0
 */
int DestructionMutex(int semid)
{
  return (semctl(semid, 0, IPC_RMID, 0));
}

/*********************	Op�ration P
 *  ENTREE: semid = identificateur de la famille de semaphores
 *  SORTIE: neant
 *  RETOUR: 0 ou SEMerr
 */
int Ps(int semid, int voie)
{
  struct sembuf semoper;
  semoper.sem_num = voie;
  semoper.sem_op = -1;
  semoper.sem_flg = 0;
  
  if (semop(semid, &semoper, 1) < 0)
    {
      perror("Erreur P sur le Mutex");
      return SEMerr;
    }
  
  return 0;
}

/*********************	Op�ration V
 *  ENTREE: semid = identificateur de la famille de semaphores
 *  SORTIE: neant
 *  RETOUR: 0 ou SEMerr
 */
int Vs(int semid, int voie)
{
  struct sembuf semoper;
  semoper.sem_num = voie;
  semoper.sem_op = 1;
  semoper.sem_flg = 0;
  
  if (semop(semid, &semoper, 1) < 0)
    {
      perror("Erreur V sur le Mutex");
      return SEMerr;
    }
  
  return 0;
}
