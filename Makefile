CC=g++
CFLAGS=-Wall

all: to-html

to-html: to-html.o
	$(CC) $(CFLAGS) bin/$^ -o bin/$@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o bin/$@

clean:
	rm -f bin/*

clean-html:
	rm -f html/*

clean-queries:
	rm -f queries/*

clean-symlinks:
	rm -f grid-prev.html rand-prev.html

# Cleans all binaries and clears cache
purge: clean clear-html clear-queries

