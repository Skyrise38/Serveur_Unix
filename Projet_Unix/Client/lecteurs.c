#include "lecteurs.h"

void lecteur_1(SEMAPHORE sem1, int semid, BUF *ptr_tampon, int* pfd1 )
{               int val1;
                char str_lecteur1[128];
                P(sem1);
                /*printf("J'ai recu un signal1 \n");*/
                Ps(semid,0);
                val1 = ptr_tampon->tampon[ptr_tampon->n];
                sprintf(str_lecteur1, "%d le ", ptr_tampon->tampon[ptr_tampon->n]);
                time_t temps_actuel;
                struct tm *heure_locale;
                
                char chaine[128];
                temps_actuel = time(NULL);
                heure_locale = localtime(&temps_actuel);

                strftime(chaine, 128, "%A %d %B %Y %H:%M:%S\n", heure_locale);
                strcat(str_lecteur1, chaine);
                Vs(semid,0);
                write(pfd1[1], str_lecteur1,sizeof(str_lecteur1)+1);

}

void lecteur_2(SEMAPHORE sem2, int semid, BUF *ptr_tampon, int* pfd2 )
{              
            int val2;
            char str_lecteur2[128];
            P(sem2);
            /*printf("J'ai recu un signal2\n");*/
            Ps(semid,1);
            val2 = (ptr_tampon+1)->tampon[(ptr_tampon+1)->n];
            sprintf(str_lecteur2, "%d le ", (ptr_tampon+1)->tampon[(ptr_tampon+1)->n]);

            /*  Récuperation de la date et stockage dans str_lecteur*/
            time_t temps_actuel;
            struct tm *heure_locale;
            char chaine[128];
            temps_actuel = time(NULL);
            heure_locale = localtime(&temps_actuel);
            strftime(chaine, 128, "%A %d %B %Y %H:%M:%S\n", heure_locale);
            strcat(str_lecteur2, chaine);

            Vs(semid,1);
            write(pfd2[1], str_lecteur2,sizeof(str_lecteur2)+1);/*Envoie dans le pipe*/
}

