STANDALONE := ll.bin

all: ll.o

standalone:
	gcc -o $(STANDALONE) -D STANDALONE ll.c -g

standalone_cpp:
	ln -sv ll.c ll.cpp
	g++ -o $(STANDALONE) -D STANDALONE ll.cpp -g

clean:
	rm -f *.o
	rm -f $(STANDALONE) 
	rm -f ll.cpp
