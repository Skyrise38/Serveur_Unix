#include "redacteurs.h"

void redacteur_1(int* pfd1, int* pfddriver, pthread_mutex_t* thread_id )
{   
    char read1[128];
    char str_redacteur1[700]= {};
    int i;
    for (i = 0; i < 5; ++i)
    {
        read(pfd1[0],read1,sizeof(read1)+1);
        strcat(str_redacteur1,read1);
        
    }
    /*printf("str_redacteur1 :\n%s", str_redacteur1);*/
    pthread_mutex_lock(thread_id);
    write(pfddriver[1], str_redacteur1,sizeof(str_redacteur1)+1);
    pthread_mutex_unlock(thread_id);
}
void redacteur_2(int* pfd2, int* pfddriver, pthread_mutex_t* thread_id)
{   char read2[128];
    char str_redacteur2[700]= {};
    int i;
    for (i = 0; i < 5; ++i){

        read(pfd2[0],read2,sizeof(read2)+1);
        strcat(str_redacteur2,read2);
    }
    /*printf("str_redacteur2 :\n%s", str_redacteur2);*/
    pthread_mutex_lock(thread_id);
    write(pfddriver[1], str_redacteur2,sizeof(str_redacteur2)+1);
    pthread_mutex_unlock(thread_id);
}
