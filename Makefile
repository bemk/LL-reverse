STANDALONE := ll.bin

all: ll.o

standalone:
	gcc -o $(STANDALONE) -D STANDALONE ll.c -g

clean:
	rm -f *.o
	rm -f $(STANDALONE) 
