CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

walk: walk.o rkhosa.o cschmiedel.o libggfonts.a
	g++ walk.o rkhosa.o libggfonts.a -Wall -Wextra $(LFLAGS) -o walk

walk.o: walk.cpp rkhosa.h
	g++ $(CFLAGS) -c walk.cpp -Wall -Wextra

rkhosa.o: rkhosa.cpp rkhosa.h
	g++ $(CFLAGS) -c rkhosa.cpp -Wall -Wextra

cschmiedel.o: cschmiedel.cpp cschmiedel.h
	g++ $(CFLAGS) -c cschmiedel.cpp -Wall -Wextra


clean:
	rm -f walk *.o
