single:
	gcc singleThread.c fileIO.c format.c -o singleThread
	./singleThread input.txt 4 55 25 4

multi:
	gcc multiThread.c fileIO.c format.c -o multiThread