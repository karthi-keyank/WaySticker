CC = gcc

PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

CFLAGS = $(shell pkg-config --cflags gtk4 gtk4-layer-shell-0 json-c)
LIBS = $(shell pkg-config --libs gtk4 gtk4-layer-shell-0 json-c)

RENDERER_SRC = \
	src/renderer/main.c \
	src/renderer/window.c \
	src/common/config.c

EDITOR_SRC = \
	src/editor/main.c \
	src/editor/editor.c \
	src/editor/gallery.c \
	src/editor/thumbnail.c \
	src/editor/chip.c \
	src/editor/style.c \
	src/editor/renderer.c \
	src/editor/drag.c \
	src/editor/resize.c \
	src/common/config.c

RENDERER_BIN = stickers-render
EDITOR_BIN = stickers-editor

all: renderer editor

renderer:
	$(CC) $(RENDERER_SRC) \
	-o $(RENDERER_BIN) \
	$(CFLAGS) \
	$(LIBS)

editor:
	$(CC) $(EDITOR_SRC) \
	-o $(EDITOR_BIN) \
	$(CFLAGS) \
	$(LIBS)

install: all
	install -Dm755 $(RENDERER_BIN) $(DESTDIR)$(BINDIR)/$(RENDERER_BIN)
	install -Dm755 $(EDITOR_BIN) $(DESTDIR)$(BINDIR)/$(EDITOR_BIN)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(RENDERER_BIN)
	rm -f $(DESTDIR)$(BINDIR)/$(EDITOR_BIN)

run-renderer: renderer
	./$(RENDERER_BIN)

run-editor: editor
	./$(EDITOR_BIN)

clean:
	rm -f $(RENDERER_BIN) $(EDITOR_BIN)

.PHONY: all renderer editor install uninstall clean run-renderer run-editor