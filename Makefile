uelp: src/main.c
	gcc -o uelp src/main.c -lsqlite3

clean:
	rm uelp books.db
