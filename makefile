all:
	g++ -c -std=c++17 main.cpp
	g++ main.o -o pong -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./pong