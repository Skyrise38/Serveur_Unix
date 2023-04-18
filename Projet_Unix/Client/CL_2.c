

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
    
    int pid_redac2;
    int pid_redac1;
    
    
    char read1[128];
    char read2[128];



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



    /* Création du pipe entre lecteur 1 et rédacteur 1 */
    if (pipe(pfd1)==-1)
        printf("Erreur Pipe 1 \n");
            
    /*Création du pipe entre lecteur 2 et rédacteur 2*/
    if (pipe(pfd2)==-1)
        printf("Erreur Pipe 2\n");
            
    /* Creation du lecteur 1 */

    pid_1 = fork();

    if (pid_1 > 0) /* Code execute par le Moniteur  */ 
    {
        //printf("Moniteur " : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);


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
                        char str_redacteur2[700]= {};
                        int i;
                        for (i = 0; i < 5; ++i){

                            read(pfd2[0],read2,sizeof(read2)+1);
                            strcat(str_redacteur2,read2);
                        }
                        printf("str_lecteur2 :\n%s", str_redacteur2);
                    }

                }
                
            }        
            else {
                /*Code Rédacteur 1*/

                close(pfd1[1]);
                while(1)
                {   
                    char str_redacteur1[700]= {};
                    int i;
                    for (i = 0; i < 5; ++i)
                    {
                        read(pfd1[0],read1,sizeof(read1)+1);
                        strcat(str_redacteur1,read1);
        
                    }
                    printf("str_lecteur1 :\n%s", str_redacteur1);
                }
            } 

            for(i=0;i<30;i++)
            {
                    /*printf("Pere : \tmoi=%d\tfils1=%d\tfils2=%d\n", pid_p, pid_1, pid_2);*/
                    pause();
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
    Detruire_sem(sem1);
    Detruire_sem(sem2);
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

