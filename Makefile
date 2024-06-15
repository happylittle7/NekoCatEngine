all:
	-gcc master.c ./Core/toml.c ./Core/dialog.c ./Core/backpack.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o master
clean:
	rm -f master