CC = gcc

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

all: renderer editor

renderer:
	$(CC) $(RENDERER_SRC) \
	-o stickers-render \
	$(CFLAGS) \
	$(LIBS)

editor:
	$(CC) $(EDITOR_SRC) \
	-o stickers-editor \
	$(CFLAGS) \
	$(LIBS)

run-renderer: renderer
	./stickers-render

run-editor: editor
	./stickers-editor

clean:
	rm -f stickers-render stickers-editor