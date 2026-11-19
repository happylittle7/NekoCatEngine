CC ?= gcc
CFLAGS ?= -O2
SRCS = master.c Library/toml.c Core/dialog.c Core/openning.c Core/backpack.c
HDRS = $(wildcard Core/*.h Library/*.h)
LIBS = `sdl2-config --cflags --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf

all: main

# Fail loudly: the old "-gcc" ignored compile errors and kept running a stale binary.
main: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o main $(LIBS)

clean:
	rm -f main

.PHONY: all clean
