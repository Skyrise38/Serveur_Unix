char* connection_msg(int msqid);
void deconection_msg(int msqid);
key_t Creer_cle(char *nom_fichier);
int CreationMessagerie();
BUF* connection_shm(char* cle_shm);
void detachement_shm(BUF* ptr_shm);


void my_handler1(int n);
void my_handler2(int n);





