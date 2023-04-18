#include "memoire_partagee.h"


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