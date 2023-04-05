#include "CL_include"
#include "CL_2.h"

BUF *ptr_tampon;
SEMAPHORE sem1;
SEMAPHORE sem2;
int semid;


int main(){

    char cle_shm[L_MSG];
    int msqid;
    int i;
    key_t cle1;
    key_t cle2;

    int pid_p;
    int pid_1 = -1;
    int pid_2 = -1;
    int no;
    int pid_redac2;
    int pid_redac1;
  
    int my_pid;
    my_pid = getpid();
    printf("PID Client : %d \n", my_pid);

    msqid=CreationMessagerie();
    strcpy(cle_shm, connection_msg(msqid));
  
    ptr_tampon=connection_shm(cle_shm);
  

    signal(SIGUSR1, my_handler1);
    signal(SIGUSR2, my_handler2);
    


    /* Code execute par le Pere */
    pid_p = getpid();

    /* On genere une cle */
    cle1 = Creer_cle("CL_2.c");

    /* On genere une deuxième cle */
    cle2 = Creer_cle("CL_1.c");

    /* On cree un semaphore initialise a 0 */
    sem1=Creer_sem(cle1);
    
    sem2=Creer_sem(cle2);

    //Connexion au sémaphore du serveur
    semid=CreationMutex();


    /* Creation du premier fils */

    pid_1 = fork();

    if (pid_1 > 0) /* Code execute par le Pere */ 
    {
        printf("Pere : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);


        /* Creation du deuxieme fils */
        pid_2 = fork();
        if (pid_2 > 0) /* Code execute par le Pere */ 
        {   
            for(i=0;i<10;i++)
            {
                printf("Pere : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);
                pause();
            }
        } 
        else /* Code du 2eme fils */ 
        {
            my_pid = getpid();
            pid_redac2 = fork();
            if (pid_redac2>0){
                //Code Rédacteur 2

            }
            while(1)
            {
                P(sem1);
                printf("J'ai recu un signal1\n");
                Ps(semid,0);
                printf("La valeur sur la voie 1 est : %d \n", ptr_tampon->tampon[ptr_tampon->n]); 
                Vs(semid,0);
            }

        }
    } 
    else/* Code du 1er fils */ 
    {
        my_pid = getpid();
        pid_redac2 = fork();
        if (pid_redac2>0){
                //Code Rédacteur 2

            }
        while(1)
        {
            P(sem2);
            printf("J'ai recu un signal2\n");
            Ps(semid,1);
            printf("La valeur sur la voie 2 est : %d \n", (ptr_tampon+1)->tampon[(ptr_tampon+1)->n]);
            Vs(semid,1);
            
        }
    }
    kill(pid_1, SIGKILL);
    kill(pid_2, SIGKILL);
    Detruire_sem(sem1);
    Detruire_sem(sem2);
    detachement_shm(ptr_tampon);
    deconection_msg(msqid);
    
}




//////////****************** Fonctions *****************///////////

key_t Creer_cle(char *nom_fichier)
{
  key_t cle;

  if ( (cle = ftok(nom_fichier,'0')) == -1)
    {
      perror("Creer_cle");
      exit(EXIT_FAILURE);
    }
  return cle;
}

int CreationMessagerie()
{
    int code_erreur;
    key_t key;
    int  msqid;
    if ((code_erreur = ( key = ftok(CleServeur,'M')) < 0) ){
        printf("Erreur : %d ",code_erreur);
        return CLEerr;
    }
        
    if ((msqid = msgget(key,0666))<0){
        printf("Erreur msqid ");
        
    }
    printf("Msqid : %d \n", msqid);
    return msqid;
}


void deconection_msg(int msqid){
    
    dmsgbuf msg;
    int code_erreur;
    int CPID= getpid();
    
    /*Deconnection de la messagerie */
    msg.type =DECONNECT;
    sprintf(msg.txt,"%d",CPID);
    if ((code_erreur = msgsnd(msqid,&msg,strlen(msg.txt)+1,0))<0){
        printf("Erreur de la deconnection de la messagerie : %d",code_erreur);  
    }
}


char* connection_msg(int msqid){
     
    dmsgbuf msg,receive;
    int code_erreur;
    int CPID= getpid();
    static char cle_shm[L_MSG];
    
     /* Connection à la messagerie et envoie du PID*/
    msg.type =CONNECT;
    sprintf(msg.txt,"%d",CPID);
    if ((code_erreur = msgsnd(msqid,&msg,strlen(msg.txt)+1,0))<0){
        printf("Erreur de connection à la messagerie, code_erreur : %d",code_erreur);
        return NULL;
    }
    
    /*Reception de la reponse du serveur*/
    receive.type= CPID;
    if ((code_erreur = msgrcv(msqid,&receive,L_MSG,receive.type,0))<0){
        printf("Erreur de reception de la clé d'accès à la memoire partagé, code_erreur : %d",code_erreur); 
        return NULL;
    }
    strcpy (cle_shm,receive.txt);
    printf ("Clé d'accès à la memoire partagée %s \n", cle_shm);
    
    /*Envoie ACK*/
    msg.type =ACK;
    sprintf(msg.txt,"%d",CPID);
    
    if ((code_erreur = msgsnd(msqid,&msg,strlen(msg.txt)+1,0))<0){
        printf("Erreur de l'envoie de l'ACK, code_erreur : %d",code_erreur); 
        
    }
    return cle_shm;
}  
BUF* connection_shm(char* cle_shm){
    /*transformation de la cle de memoire partagé retourné par le serveur lors de la connection à la messagerie en cle de type key_t */
    key_t cle;
    int CLTshmid;
    BUF *ptr_tampon;
    
    if ((cle =ftok(cle_shm,'T'))<0){
        printf("Erreur de creation clé memoire partagée :%d \n",CLEerr);
    }
    /*Creation de la memoire partagée */
    if ((CLTshmid=shmget(cle, 2*sizeof(BUF), 0666))<0){
        printf("Erreur de creation memoire partagée :%d \n",errno);
    }
    /*Attachement à la memoire partagée */
    if((ptr_tampon = (BUF *) shmat (CLTshmid, 0,0))==0){
        printf("Erreur d'attachement à la memoire partagée :%d \n",SHMerr);
    }
   
    printf("Tshmid= %d  Pointeur Tampon= 0x%x\n",CLTshmid,(unsigned) ptr_tampon);
    return ptr_tampon;
}
void detachement_shm(BUF* ptr_shm){
    if((shmdt(ptr_shm)<0)){
        printf("erreur");
    }
}

void my_handler1(int n){
    V(sem1);
} 

void my_handler2(int n){
    V(sem2); 
    
} 

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
