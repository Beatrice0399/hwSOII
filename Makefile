main:
	gcc singleThread.c fileIO.c path.c format.c -o singleThread
	./singleThread input.txt 3 55 30 4