# flags per la compilazione
CFLAGS = -std=c89 -Wpedantic
# eseguibile
TARGET = Main
TARGET1 = Player 
TARGET2 = Pedina 
# object files necessari per produrre l'eseguibile
OBJ    = Libreria.o Main.c
OBJ1   = Libreria.o Player.c
OBJ2    = Libreria.o Pedina.c

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -g -O0 -o  $(TARGET)

$(TARGET1): $(OBJ1)
	$(CC) $(OBJ1) $(CFLAGS) -g -O0 -o  $(TARGET1)

$(TARGET2): $(OBJ2)
	$(CC) $(OBJ2) $(CFLAGS) -g -O0 -o  $(TARGET2)


all: $(TARGET) $(TARGET1) $(TARGET2)

clean:
	rm -f *.o $(TARGET) $(TARGET1) $(TARGET2) *~

run: $(TARGET) $(TARGET1) $(TARGET2)
	./$(TARGET)
