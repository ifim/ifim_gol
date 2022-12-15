all: main
	mv main ifim_gol

main: ifim_gol_config.o ifim_gol_board.o

clean:
	rm ifim_gol *.o
