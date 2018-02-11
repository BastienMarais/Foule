#changer le nom des deux lignes suivantes si nécessaire
SOURCE = foule_V5.c
OUTPUT = resultat


SDL_CFLAGS = $(shell sdl-config --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs)


all: $(OUTPUT)

$(OUTPUT): ../lib/libgraphique.o ../lib/lib_foule.o source.o
	@gcc ../lib/libgraphique.o ../lib/lib_foule.o source.o -o $(OUTPUT)  $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm -lSDL -lSDL_ttf


../lib/libgraphique.o: ../lib/libgraphique.c
	@gcc   -c ../lib/libgraphique.c -o ../lib/libgraphique.o 

../lib/lib_foule.o: ../lib/lib_foule.c
	@gcc   -c ../lib/lib_foule.c -o ../lib/lib_foule.o 

source.o: $(SOURCE)
	@gcc -Wall -c $(SOURCE)  -o source.o






