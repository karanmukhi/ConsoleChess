output: main.o game.o player.o board.o
	g++ main.o game.o player.o board.o -o run

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

game.o: game.cpp game.hpp
	g++ -std=c++14 -c game.cpp

player.o: player.cpp player.hpp
	g++ -std=c++14 -c player.cpp

board.o: board.cpp board.hpp
	g++ -std=c++14 -c board.cpp

clean:
	rm *.o run