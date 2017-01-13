CXX ?= g++

all:
	$(CXX) asteroide.cpp colpo.cpp firststage.cpp global.cpp main.cpp menu.cpp nave.cpp -Ipega -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lGLEW -lGL -lGLU -lzip -o firstgame
