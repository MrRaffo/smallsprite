#Compiler
CC = gcc

#Compiler flags
FLAGS = -g -Wall

#Linked libraries
LINKS = -lSDL2 -lSDL2main -lm

#OUTPUT FILE
OUTPUT = smallsprite

#INPUT
INPUT = main.o utility.o graphics.o gui.o palette.o sprite.o

#FILES and DEPENDANCIES
$(OUTPUT): $(INPUT)
	$(CC) $(INPUT) $(FLAGS) $(LINKS) -o $(OUTPUT)

main.o: main.c
	$(CC) main.c $(FLAGS) $(LINKS) -c

graphics.o: graphics.c
	$(CC) graphics.c $(FLAGS) $(LINKS) -c

utility.o: utility.c
	$(CC) utility.c $(FLAGS) $(LINKS) -c

gui.o: gui.c
	$(CC) gui.c $(FLAGS) $(LINKS) -c

palette.o: palette.c
	$(CC) palette.c $(FLAGS) $(LINKS) -c

sprite.o: sprite.c
	$(CC) sprite.c $(FLAGS) $(LINKS) -c

clean:
	rm -f $(INPUT)

cleanall:
	rm -f $(INPUT) $(OUTPUT)