all:
	-gcc main.c ./Core/backpack.c -o main `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
	-gcc master.c core/toml.c core/dialog.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o master
clean:
	rm -f main master