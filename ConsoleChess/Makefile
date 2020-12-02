output: main.o game.o player.o board.o piece.o 
	g++ main.o game.o player.o board.o piece.o -o run

main.o: main.cpp
	g++ -c main.cpp

game.o: game.cpp game.hpp
	g++ -c game.cpp

player.o: player.cpp player.hpp
	g++ -c player.cpp

board.o: board.cpp board.hpp
	g++ -c board.cpp

piece.o: piece.cpp piece.hpp
	g++ -c piece.cpp

clean:
	rm *.o output