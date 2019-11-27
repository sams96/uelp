uelp: src/main.c
	gcc -o uelp src/main.c -lsqlite3 `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm uelp books.db
