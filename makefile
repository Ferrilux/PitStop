all: Pit

Pit: *.cpp
	g++ *.cpp -o Pit -lncursesw -g 

rm:
	rm Pit