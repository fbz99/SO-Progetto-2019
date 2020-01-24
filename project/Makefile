# dal carattere '#' fino a fine riga, il testo dentro il Makefile e`
# un commento
#
# flags per la compilazione
CFLAGS = -std=c89 -pedantic -g


# con "make clean" solitamente si vogliono eliminare i prodotti della
# compilazione e ripristinare il contenuto originale
all: Libreria main bandierina player pedina 


clean:
	rm -f *.o main bandierina player pedina *~

Libreria:
	gcc -c Libreria.c 

main: main.c  Makefile
	gcc $(CFLAGS) main.c Libreria.o -o main

player: player.c Makefile
	gcc $(CFLAGS) player.c Libreria.o -o player


bandierina: bandierina.c Makefile
	gcc $(CFLAGS) bandierina.c Libreria.o -o bandierina
	
pedina: pedina.c Makefile
	gcc $(CFLAGS) pedina.c Libreria.o -o pedina

# il target run si usa talvolta per eseguire l'applicazione
run:  all
	./main

	
