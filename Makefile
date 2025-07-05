CC      = g++
CFLAGS  = -std=c++17 -Wall 
LINKS = -D_GNU_SOURCE

main: src/main.o
	$(CC) $(CFLAGS) $(LINKS) -o main src/main.o

main.o: src/main.cpp
	$(CC) $(CFLAGS) $(LINKS) -c src/main.cpp

clean:
	rm -f hello main.o

.PHONY: clean
