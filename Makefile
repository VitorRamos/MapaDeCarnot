
all: main
main: main.cpp
	g++ main.cpp -lGL -lGLU -lglut -o main

.PHONY: clean

clean:
	-rm main

