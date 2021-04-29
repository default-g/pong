all:
	g++ -c main.cpp
	g++ main.o -o pong -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./pong