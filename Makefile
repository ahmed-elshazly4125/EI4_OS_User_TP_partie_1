CC = gcc
CFLAGS = -Wall
LDFLAGS = -lreadline

TARGET = biceps

all: $(TARGET)

$(TARGET): biceps.o
	$(CC) biceps.o -o $(TARGET) $(LDFLAGS)

biceps.o: biceps.c
	$(CC) $(CFLAGS) -c biceps.c

clean:
	rm -f $(TARGET) *.o