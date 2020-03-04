all:
	gcc src\\*.c -o bin\\main.exe

run:
	gcc src\\*.c -o bin\\main
	bin\\main