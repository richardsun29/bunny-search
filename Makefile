CC=g++
CFLAGS=-Wall

all: to-html

to-html: to-html.o
	$(CC) $(CFLAGS) bin/$^ -o bin/$@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o bin/$@


# Clean binaries and clear cache
clean:
	rm -f bin/ html/ queries/ grid-prev.html rand-prev.html
