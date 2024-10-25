NAME=locus-shell
BIN=${NAME}
SRC=.

PKGS = locus librsvg-2.0 libnm upower-glib

LOCUS_SHELL_SOURCES += $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/**/*.c)
LOCUS_SHELL_HEADERS += $(wildcard $(SRC)/*.h) $(wildcard $(SRC)/**/*.h)

CFLAGS += -std=gnu99 -Wall -g -Wno-format-truncation
CFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS)) -lm -lutil -lrt

SOURCES = $(LOCUS_SHELL_SOURCES)

OBJECTS = $(SOURCES:.c=.o)

all: ${BIN}

$(OBJECTS): $(LOCUS_SHELL_HEADERS)

$(BIN):$(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) ${BIN}

format:
	clang-format -i $(LOCUS_SHELL_SOURCES) $(LOCUS_SHELL_HEADERS)
