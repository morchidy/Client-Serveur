CC = gcc
CFLAG = -ansi -Wall -D _DEFAULT_SOURCE -std=c99

# Liste des fichiers objets nécessaires pour chaque exécutable
OBJS_CL = CL.o moniteur.o lecteur.o redacteur.o driver.o
OBJS_DRIVER = driver.o

# Règle pour générer l'exécutable CL
CL: $(OBJS_CL)
	$(CC) $(CFLAG) -o CL $(OBJS_CL)

# Règle pour générer l'exécutable driver
driver: $(OBJS_DRIVER)
	$(CC) $(CFLAG) -o driver $(OBJS_DRIVER)

# Règle de compilation pour chaque fichier .c
CL.o: CL.c moniteur.h lecteur.h redacteur.h driver.h
	$(CC) $(CFLAG) -c CL.c

moniteur.o: moniteur.c moniteur.h
	$(CC) $(CFLAG) -c moniteur.c

lecteur.o: lecteur.c lecteur.h
	$(CC) $(CFLAG) -c lecteur.c

redacteur.o: redacteur.c redacteur.h
	$(CC) $(CFLAG) -c redacteur.c

driver.o: driver.c driver.h
	$(CC) $(CFLAG) -c driver.c

# Règle de nettoyage
clean:
	@rm -f *~ *.o driver CL
