all:
	g++ *.cpp -lncurses -o ntris
clean:
	rm -f *.o ntris
