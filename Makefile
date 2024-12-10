# Cible par défaut
all: clean TetrisLinux

# Compilation des fichiers objets
main.o: main.cpp
	g++ -c main.cpp

bloc.o: bloc.cpp
	g++ -c bloc.cpp

menu.o: menu.cpp
	g++ -c menu.cpp

# Lien pour créer l'exécutable
TetrisLinux: main.o bloc.o menu.o
	g++ -g -o TetrisLinux main.o bloc.o menu.o -lsfml-graphics -lsfml-window -lsfml-system -lGL -lpthread -lfreetype -ljpeg

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f *.o TetrisLinux



# #Variables
# CXX = x86_64-w64-mingw32-g++
# CXXFLAGS = -I/home/Lucas/SFML-Lib/include -std=c++17 -Wall -DSFML_STATIC
# LDFLAGS = -L/home/Lucas/SFML-Lib/lib -static -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lgdi32 -lwinmm -lkernel32 -lfreetype -ljpeg
# SOURCES = main.cpp bloc.cpp menu.cpp
# OBJECTS = $(SOURCES:.cpp=.o)
# EXECUTABLE = TetrisWindows.exe

# # Règles
# window: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CXX) -o $@ $^ $(LDFLAGS)

# %.o: %.cpp	
# 	$(CXX) $(CXXFLAGS) -c $< -o $@
