all:
	-gcc openning.c ./Library/text.c -o openning `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
clean:
	rm -f openning