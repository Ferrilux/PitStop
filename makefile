all: Pit

Pit: Pit.cpp
	g++ Pit.cpp -o Pit -lncurses

rm:
	rm Pit