all:
	-gcc master.c ./Library/toml.c  ./Core/dialog.c ./Core/openning.c ./Core/backpack.c -o main `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
clean:
	rm -f main
