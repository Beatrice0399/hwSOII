main:
	gcc singleThread.c fileIO.c path.c format.c -o singleThread
	./singleThread input.txt 3 33 28 4