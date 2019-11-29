CC = gcc
CSTD = c99

XML_CFLAGS = $(shell xml2-config --cflags)
CFLAGS = -g -W -Wall -pedantic -std=$(CSTD) $(XML_CFLAGS)

XML_LDFLAGS = $(shell xml2-config --libs)
LDFLAGS = -lsqlite3 -lzip $(XML_LDFLAGS)

SOURCES = $(shell find src/ ! -name "test.c" -name '*.c')
DEPS = $(shell find src/ -name '*.h')
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
EXECUTABLE = uelp

$(EXECUTABLE): pre $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

pre:
	@echo -e "C compiler: \t" $(CC) 
	@echo -e "C flags: \t" $(CFLAGS)
	@echo -e "Library flags: \t" $(LDFLAGS)

%.o: %.c $(DEPS)
	@echo "Building" $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJECTS) $(EXECUTABLE) books.db
