single:
	gcc singleThread.c fileIO.c format.c -o singleThread

multi:
	gcc multiThread.c fileIO.c format.c -o multiThread