main:
	gcc main.c -o main
	./main Input2.txt 3 33 28 4

single:
	gcc single.c fileIO.c formatText.c path.c -o single
	./single Input2.txt 3 33 28 4

hw: 
	gcc hw.c -o hw 
	./hw 