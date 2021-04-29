all:
	g++ -c main.cpp -std=c++2a
	g++ main.o -o pong -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./pong