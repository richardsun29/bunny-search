CC=g++
CFLAGS=-Wall

all: to-html

to-html: to-html.o
	$(CC) $(CFLAGS) bin/$^ -o bin/$@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o bin/$@

clean:
	rm -f bin/*

clear-html:
	rm -f html/*

clear-queries:
	rm -f queries/*

# Cleans all binaries and clears cache
purge: clean clear-html clear-queries

