# dal carattere '#' fino a fine riga, il testo dentro il Makefile e`
# un commento
#
# flags per la compilazione
CFLAGS = -std=c89 -pedantic -g

# con "make clean" solitamente si vogliono eliminare i prodotti della
# compilazione e ripristinare il contenuto originale
all: main bandierina player pedina 


clean:
	rm -f *.o main bandierina player pedina *~

Libreria:
	gcc -c Libreria.c 

main: main.c  Makefile
	gcc $(CFLAGS) main.c -o main

player: player.c Makefile
	gcc $(CFLAGS) player.c -o player


bandierina: bandierina.c Makefile
	gcc $(CFLAGS) bandierina.c -o bandierina
	
pedina: pedina.c Makefile
	gcc $(CFLAGS) pedina.c -o pedina

# il target run si usa talvolta per eseguire l'applicazione
run:  all
	./main

	
