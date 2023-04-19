

#include "CL_include"
#include "CL_2.h"

BUF *ptr_tampon;
SEMAPHORE sem1;
SEMAPHORE sem2;

int main(){

    char cle_shm[L_MSG];
    int msqid;
    int i;
    key_t cle1;
    key_t cle2;

    int pid_p;
    int pid_1 = -1;
    int pid_2 = -1;


    int semid;

    int pfd1[2];
    int pfd2[2];
    int pfddriver[2];
    
    int pid_redac2;
    int pid_redac1;

    int pid_driver;
    
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

    /*Connexion au sémaphore du serveur*/
    semid=CreationMutex();

    /*Mutex driver*/
    pthread_mutex_t thread_id;
    pthread_mutex_init(&thread_id, NULL);

    /* Création du pipe lecteur 1 -> rédacteur 1 */
    if (pipe(pfd1)==-1)
        printf("Erreur Pipe 1 \n");
            
    /*Création du pipe lecteur 2 -> rédacteur 2*/
    if (pipe(pfd2)==-1)
        printf("Erreur Pipe 2\n");
    
    /* Creation du pipe Redacteur1/2 -> Driver */
    if (pipe(pfddriver)==-1)
        printf("Erreur Pipe Driver\n");

    /* Creation du lecteur 1 */
    pid_1 = fork();

    if (pid_1 > 0) /* Code execute par le Moniteur  */ 
    {
        /*printf("Moniteur " : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);*/
        /* Creation du lecteur 2 */
        pid_2 = fork();
        if (pid_2 > 0) /* Code execute par le Moniteur */ 
        {   
            pid_redac1 = fork(); /* Creation Redacteur 1 */

            if (pid_redac1>0)
            {   /* Code Moniteur */ 
                pid_redac2 = fork(); /* Creation Redacteur 2*/
                if (pid_redac2>0){

                }
                else {
                    /*Code Rédacteur 2*/

                    close(pfd2[1]);
                    while(1)
                    {   
                        redacteur_2(pfd2, pfddriver,&thread_id);
                    }

                }
            }        
            else {
                /*Code Rédacteur 1*/

                close(pfd1[1]);
                while(1)
                {   
                    redacteur_1(pfd1,pfddriver, &thread_id);
                }
            }

            pid_driver=fork(); /* Creation du driver */
            /* Code du Moniteur */
            if (pid_driver>0)
            {
                for(i=0;i<30;i++)
                {
                        /*printf("Pere : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);*/
                        pause();
                }
            }
            /* Code du Driver*/
            else
            {
                while(1)
                {
                    driver(pfddriver);
                }
                
            }
            
        }    
        else /* Code du lecteur 1 */ 
        {
            my_pid = getpid();
            
            while(1)
            {
              lecteur_1(sem1, semid,ptr_tampon, pfd1 );
            }

        }
    } 
    else /* Code du lecteur 2 */ 
    {
        my_pid = getpid();

        while(1)
        {
          lecteur_2(sem2, semid, ptr_tampon, pfd2 );
        }
    }
    kill(pid_redac1, SIGKILL);
    kill(pid_redac2, SIGKILL);
    kill(pid_1, SIGKILL);
    kill(pid_2, SIGKILL);
    kill(pid_driver, SIGKILL);
    Detruire_sem(sem1);
    Detruire_sem(sem2);
    pthread_mutex_destroy(&thread_id);
    detachement_shm(ptr_tampon);
    deconection_msg(msqid);
}
/* ---------------- Fonctions d'interruptions ----------------*/
void my_handler1(int n){
    V(sem1);
} 

void my_handler2(int n){
    V(sem2); 
    
} 

