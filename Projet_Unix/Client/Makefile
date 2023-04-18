# Nom du programme à générer
TARGET = CL

CC = gcc
CFLAGS = -g -Wall -ansi -D _GNU_SOURCE 
CFLAG2 = -c -Wall -ansi -D _GNU_SOURCE 


# Fichiers sources
SRCS = CL_2.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle de construction du programme
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

# Règle de compilation des fichiers objets
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


# Règle de nettoyage
clean:
	rm -f $(OBJS) $(TARGET)