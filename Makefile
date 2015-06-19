CC=g++
CFLAGS=-Wall

all: to-html

to-html: to-html.o
	$(CC) $(CFLAGS) obj/$^ -o bin/$@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o obj/$@

clean:
	rm -f obj/*.o bin/* *.o

# Cleans all binaries and clears cache
purge: clean
	rm -f data/html/*
	rm -f data/queries/*
	rm -f data/urls.txt
