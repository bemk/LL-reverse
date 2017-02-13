STANDALONE_C := ll.c.bin
STANDALONE_CPP := ll.cpp.bin

all: ll.o standalone standalone_cpp

standalone:
	gcc -o $(STANDALONE_C) -D STANDALONE ll.c -g

standalone_cpp:
	ln -sv ll.c ll.cpp
	g++ -o $(STANDALONE_CPP) -D STANDALONE ll.cpp -g

clean:
	rm -f *.o
	rm -f $(STANDALONE_C) 
	rm -f $(STANDALONE_CPP)
	rm -f ll.cpp
