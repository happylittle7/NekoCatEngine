all:
	gcc main.c Library/toml.c -o main
clean:
	rm -rf main
	rm -rf *.out