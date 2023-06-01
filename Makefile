main:
	gcc main.c -o main
	./main Input2.txt 3 33 28 4

single:
	gcc single_thread.c strman.c journal.c fileio.c -o single_thread.o
	./single_thread.o input.txt 4 20 27 10

hw: 
	gcc hw.c -o hw 
	./hw 

s: 
	gcc singleT.c fileIO.c path.c format.c -o singleT
	./singleT input.txt 4 50 27 4