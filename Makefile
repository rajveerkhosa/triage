CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: triage

triage: walk2.cpp log.cpp
	g++ $(CFLAGS) walk2.cpp log.cpp rkhosa.cpp sbeebe.cpp cschmiedel.cpp lgarcia.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o triage

clean:
	rm -f triage
	rm -f *.o

