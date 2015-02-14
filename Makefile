CFLAGS = -Isrc -Wall -Wextra
LDLIBS = `pkg-config allegro_monolith-debug-static-5 --libs --static`
.PHONY: all
all: examples/example
examples/example: examples/example.o src/gif.o src/lzw.o src/bitmap.o src/algif.o

clean:
	${RM} src/*.o
	${RM} examples/*.o
	${RM} examples/example
	${RM} allegro.log
