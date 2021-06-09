all: Pit

Pit: *.cpp
	g++ *.cpp -o Pit -lncurses

rm:
	rm Pit