#include "redacteurs.h"

void redacteur_1(int* pfd1 )
{   
    char read1[128];
    char str_redacteur1[700]= {};
    int i;
    for (i = 0; i < 5; ++i)
    {
        read(pfd1[0],read1,sizeof(read1)+1);
        strcat(str_redacteur1,read1);
        
    }
    printf("str_lecteur1 :\n%s", str_redacteur1);
}
void redacteur_2(int* pfd2)
{   char read2[128];
    char str_redacteur2[700]= {};
    int i;
    for (i = 0; i < 5; ++i){

        read(pfd2[0],read2,sizeof(read2)+1);
        strcat(str_redacteur2,read2);
    }
    printf("str_lecteur2 :\n%s", str_redacteur2);
}
