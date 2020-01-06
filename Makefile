CC = gcc
CSTD = c99

XML_CFLAGS = $(shell xml2-config --cflags)
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
CFLAGS = -g -W -Wall -pedantic -std=$(CSTD) $(XML_CFLAGS) $(GTK_CFLAGS)

XML_LDFLAGS = $(shell xml2-config --libs)
GTK_LDFLAGS = $(shell pkg-config --libs gtk+-3.0)
LDFLAGS = -lsqlite3 -lzip $(XML_LDFLAGS) $(GTK_LDFLAGS)

SOURCES = $(shell find src/ -name '*.c')
DEPS = $(shell find src/ -name '*.h')
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
EXECUTABLE = uelp

TEST_SOURCES = $(shell find tests/ -name '*.c') \
	       $(shell find src/ ! -name 'main.c' -name '*.c')
TEST_OBJECTS = $(patsubst %.c,%.o,$(TEST_SOURCES))
TEST_EXECUTABLE = uelp-test

$(EXECUTABLE): pre $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@

test: pre $(TEST_OBJECTS)
	@$(CC) $(TEST_OBJECTS) $(LDFLAGS) -lcmocka -o $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

pre:
	@echo -e "C compiler: \t" $(CC) 
	@echo -e "C flags: \t" $(CFLAGS)
	@echo -e "Library flags: \t" $(LDFLAGS)

%.o: %.c $(DEPS)
	@echo "Building" $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)

clean-test:
	rm $(TEST_OBJECTS) $(TEST_EXECUTABLE)
