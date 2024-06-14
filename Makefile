all:
	-gcc master.c core/toml.c core/dialog.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o a.out
