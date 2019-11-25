uelp: src/main.c
	gcc `pkg-config --cflags gtk+-3.0` -o uelp src/main.c `pkg-config --libs gtk+-3.0`

clean:
	rm uelp
