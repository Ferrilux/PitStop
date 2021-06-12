all: Pit

Pit: *.cpp
	g++ -std=c++14 *.cpp -o Pit -lncursesw -g -pthread

rm:
	rm Pit