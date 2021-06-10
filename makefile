all: Pit

Pit: *.cpp
	g++ *.cpp -o Pit -lncurses -g 

rm:
	rm Pit