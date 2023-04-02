#include "CL_include"



int main(){

    char cle_shm[L_MSG];
    int msqid;
    BUF *ptr_shm;

    msqid=CreationMessagerie();
    strcpy(cle_shm, connection_msg(msqid));
  
    ptr_shm=connection_shm(cle_shm);
  

    
    

    for(int i =0;i<5;i++){
            signal(SIGUSR1, my_handler1);
            signal(SIGUSR2, my_handler2);
            pause();
            
    }
 


    detachement_shm(ptr_shm);
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
    int shmid;
    BUF *ptr_shm;
    
    if ((cle =ftok(cle_shm,'T'))<0){
        printf("Erreur de creation clé memoire partagée :%d \n",CLEerr);
    }
    /*Creation de la memoire partagée */
    if ((shmid=shmget(cle, sizeof(BUF), 0600))<0){
        printf("Erreur de creation memoire partagée :%d \n",errno);
    }
    /*Attachement à la memoire partagée */
    if((ptr_shm = (BUF *) shmat (shmid, 0,0))==0){
        printf("Erreur d'attachement à la memoire partagée :%d \n",SHMerr);
    }
    return ptr_shm;
}
void detachement_shm(BUF* ptr_shm){
    if((shmdt(ptr_shm)<0)){
        printf("erreur");
    }
}

void my_handler1(int n){
    printf("J'ai recu un signal1\n");

    
} 

void my_handler2(int n){
    printf("J'ai recu un signal2\n"); 
    
} 
