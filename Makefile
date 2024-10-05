all: clean main


main.o: main.cpp
	g++ -c main.cpp

bloc.o: bloc.cpp
	g++ -c bloc.cpp

menu.o: menu.cpp
	g++ -c menu.cpp

main: main.o bloc.o menu.o
	g++ main.o bloc.o menu.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *.o sfml-app